/* 
 * File:   cput_c11_knn.h
 * Author: david
 *
 * Created on October 17, 2016, 9:12 PM
 */

#ifndef CPUT_C11_KNN_H
#define	CPUT_C11_KNN_H

#include "parml/base/parml_config.h"
#include "parml/base/parml.h"
#include "parml/instance_learning/knn_type.h"
#include "parml/instance_learning/knn_info.h"
#include "utilities/range1d.h"
#include "utilities/data_input.h"

#include "parframepp/parallel/task_base.h"
#include "parframepp/runtime/rt_cput.h"
#include "parframepp/base/parallel_indexed_object.h"
#include "parframepp/models/model_impl_base.h"

#ifdef PARML_DEBUG
#include "utilities/exceptions.h"
#include <string>
#endif

#include <utility>
#include <chrono>

/*#include <functional>
#include <iterator>
#include <algorithm>*/

namespace parml
{
    
/**
 * @brief C++11 threaded implementation
 * of K-nearest neighbors algorithm
 */
template<>
class Knn<knn_type::Type::CPUT_C11_KNN>: public parframepp::par_indexed_object,
                                         public parframepp::model_impl_base
{
    
    public:
        
        /**
         * @brief Constructor
         */
        Knn();
        
        /**
         * @brief For the given data set predict the value for the given data point
         * The actor does all the work and distinguishes between classification and regression.
         */
        template<typename DataMat,typename DataVec,typename Similarity,typename Actor>
        std::pair<typename Actor::return_type,KnnInfo>
        predict(const utilities::DataInput<DataMat,DataVec>& data,const DataVec& point,
                const Similarity& sim,Actor& actor);
        
        /**
         * @brief Inner class that wraps the task to be executed in a manner
         * suitable to be submitted to parframepp thread pools.
         */
        template<typename DataMat,typename DataVec,
                 typename Similarity,typename Actor> struct Task; 
       
};

/**
 * @brief Inner class definition
 */
template<typename DataMat,typename DataVec,typename Similarity,typename Actor>
struct 
Knn<knn_type::Type::CPUT_C11_KNN>::Task: public parframepp::TaskBase 
{
    public:
                
        /**
         * @brief Constructor
         */      
        Task(const utilities::DataInput<DataMat,DataVec>& datain,
             const DataVec& dp,const Similarity& s, size_type k,
             const utilities::range1d<size_type>& range);
               
                
        /**
          * @brief Return the Actor this task is using 
          */
        Actor& get_actor(){return actor_;}
                 
    protected:
                
        /**
          * @brief Implements the workings of the task
          */
        virtual parframepp::TaskBase* run()final; 
                       
        /**
          * @brief Pointer to the input data matrix
          */
        const utilities::DataInput<DataMat,DataVec>* data_;
                
        /**
          * @brief The query point
          */
        const DataVec* dp_;
                
        /**
          * @brief The object that implements the knn algorithm classification or regression.
          */
        Actor actor_;
                
                         
        /**
          * @brief Pointer to the similarity object
          */
        const Similarity* sim_;
        
         /**
          * @brief the range of indices over which the task runs
          */
        utilities::range1d<size_type> range_;
                
};

template<typename DataMat,typename DataVec,typename Similarity,typename Actor>
Knn<knn_type::Type::CPUT_C11_KNN>::Task<DataMat,DataVec,
                                        Similarity,Actor>::Task(const utilities::DataInput<DataMat,DataVec>& datain,
                                                                const DataVec& dp,const Similarity& s, size_type k,
                                                                const utilities::range1d<size_type>& range)
 :
TaskBase(),
data_(&datain),
dp_(&dp),
actor_(k),
sim_(&s),
range_(range)
{}

template<typename DataMat,typename DataVec,typename Similarity,typename Actor>
parframepp::TaskBase*
Knn<knn_type::Type::CPUT_C11_KNN>::Task<DataMat,DataVec,
                                        Similarity,Actor>::run(){
    
    actor_.resume();
    actor_(*data_,*dp_,*sim_,range_);
    return nullptr;    
}



template<typename DataMat,typename DataVec,typename Similarity,typename Actor>
std::pair<typename Actor::return_type,KnnInfo>
Knn<knn_type::Type::CPUT_C11_KNN>::predict(const utilities::DataInput<DataMat,DataVec>& data,
                                           const DataVec& point,
                                           const Similarity& sim,Actor& actor){
    
    std::chrono::time_point<std::chrono::system_clock> start, end;
    start = std::chrono::system_clock::now();
    
    size_type nrows = data.t1.n_rows(); 
    size_type k = actor.n_neighbors();
    size_type labels_size = data.t2.size();
   
#ifdef PARML_DEBUG
    //using more neighbors than the actual rows of the input matrix is an error
    using utilities::ExeLogicError;
    const std::string in_msg = "In Knn<knn_type::Type::MPI_KNN>::predict(). ";
    std::string msg = in_msg + "Number of neighbors: "+std::to_string(k)+
                               " not in [1,"+std::to_string(nrows)+")";
    Assert(((k>=1) && (k < nrows)),ExeLogicError(msg)); 
    
    msg = in_msg + "Data point is empty.";
    Assert(point.size() != static_cast<size_type>(0),ExeLogicError(msg));
    
    //labels should not be empty and should be of same size
    //as the number number of training examples
    msg = in_msg + "Labels size: "+
            std::to_string(labels_size)+" not equal to: "+std::to_string(nrows);
    
    Assert(labels_size == nrows,ExeLogicError(msg));
    
    msg = in_msg+" Parallel object does not have partitions";
    Assert(this->has_partitions(),ExeLogicError(msg));
#endif  
    
    
    KnnInfo info;
    info.n_neighbors = k;
    info.n_pts_predicted = 1;
    info.nprocs = 1;
    info.nthreads = parframepp::rt_cput::n_threads();
    info.type = knn_type::Type::CPUT_C11_KNN;
    
    //if there is only one thread then we do serially
    if(info.nthreads == 1){
        
        utilities::range1d<size_type> r(0,nrows);
        actor(data,point,sim,r);
        auto rslt = actor.get_result();
        end = std::chrono::system_clock::now();
        info.runtime = end-start;
    
        return std::make_pair(rslt,info);
    }
    else{
         
        size_type nparts = this->n_partitions();
        
        typedef Knn<knn_type::Type::CPUT_C11_KNN>::Task<DataMat,DataVec,Similarity,Actor> task_type;
        
        parframepp::model_impl_base::task_creator<task_type>::create_tasks(nparts-1,data,point,sim,k,
                                                                           this->get_partitions());
        
        //clear the tasks buffer if we
        //have tasks
        /*if(this->has_tasks()){
            
            this->clear_task_buffer();
        }
        
        //resize the number of tasks
        this->tasks_.resize(nparts-1,nullptr);
        
        
        
        //allocate space for tasks. Keep the last task for this thread
        for(size_type t=0; t<nparts-1; ++t){
          this-> tasks_[t] = new task_type(this->get_partition(t),data,point,sim,k);          
        }*/
        
        //allocate the tasks to the runtime
        parframepp::rt_cput::schedule_tasks_async(this->tasks_);
        
        //compute the local task
        task_type local_task(data,point,sim,k,this->get_partition(nparts-1));
        local_task.execute();
        
        typedef typename Actor::Pair  Pair;
        
        auto& dis_local_task = local_task.get_actor().get_distances();
        std::vector<Pair> distances;
        distances.reserve(nparts*k);
        
        std::copy(dis_local_task.begin(),dis_local_task.end(),std::back_inserter(distances));
        
        //wait for the other threads to finish 
        parframepp::task_barrier::barrier(this->tasks_);
        
        //collect the local results
        for(size_type t=0; t<this->tasks_.size(); ++t){
            auto& dis = static_cast<task_type*>(this->tasks_[t])->get_actor().get_distances();
            std::copy(dis.begin(),dis.end(),std::back_inserter(distances));
        }
        
        //sort the collected distances
        std::sort(distances.begin(),distances.end(),
                [](const Pair& p1,const Pair& p2){
                return p1.second < p2.second; });
                
        //resume the local task as we reset the 
        //distances
        local_task.get_actor().resume();
        local_task.get_actor().fillin_majority_vote(data.t2,std::move(distances));        
        auto rslt = local_task.get_actor().get_result();
        
        end = std::chrono::system_clock::now();
        info.runtime = end-start;
        return std::make_pair(rslt,info);
                
    }    
}
    
}



#endif	/* CPUT_C11_KNN_H */


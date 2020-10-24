/* 
 * File:   cput_c11_kmeasn.h
 * Author: david
 *
 * Created on October 28, 2016, 6:25 PM
 */

#ifndef CPUT_C11_KMEANS_H
#define	CPUT_C11_KMEANS_H

#include "parml/unsupervised_learning/kmeans_base.h"
#include "parframepp/base/platform_enum_type.h"
#include "parframepp/base/parallel_indexed_object.h"
#include "parframepp/models/model_impl_base.h"
#include "parframepp/parallel/task_base.h"
#include "parframepp/runtime/rt_cput.h"
#include "utilities/range1d.h"

#include <chrono>
#include <memory>

namespace parml
{
    
//forward declarations
template<parframepp::platform_type::Type t,typename DataPoint> class KMeans;
template<typename T> class ArmaVecDataSet;

/**
 * @brief Serial implementation of KMeans
 */ 
template<typename DataPoint>
class KMeans<parframepp::platform_type::Type::CPUT_C11,DataPoint>: public KMeansBase<DataPoint>,
                                                                   public parframepp::par_indexed_object
{
    
public:
    
    /**
      * @brief Constructor
      */
    KMeans(const KMeansControl& cntrl);
        
        
    /**
     * @brief Cluster the given data set
     */
    template<typename DataIn,typename Similarity,typename Initializer>
    void cluster(const DataIn& data,const Similarity& similarity,const Initializer& init);
        
};





namespace kmeans_detail
{
    
    template<> 
    struct Iteration<parframepp::platform_type::Type::CPUT_C11>: public parframepp::model_impl_base
    {


       template<typename DataIn,typename Similarity>
       typename KMeansBase<typename DataIn::Row>::ActionType 
       operator()(const DataIn& data,
                  const Similarity& sim,
                  std::vector<KMeansCluster<typename DataIn::Row> >& clusters,
                  std::vector<typename DataIn::Row>& centroids); 

       /**
        * @brief The control object
        */
       const KMeansControl& cntrl;


       /**
        * @brief Constructor
        */
       Iteration(const KMeansControl& cntrl_ );
       
       
       /**
        * @brief Create the tasks for this object. Upon creation it also
        * sets the number of clusters for each created task.
        */
       template<typename DataIn,typename Similarity>
       void create_tasks(const DataIn& data,const Similarity& sim,
                         const std::vector<utilities::range1d<size_type> >& parts);
       
       
       /**
        * @brief Merge into clusters the given local clusters
        */
       template<typename RowType>
       void merge_clusters(std::vector<KMeansCluster<RowType> >& clusters,
                           const std::vector<KMeansCluster<RowType> >& localclusters);
       
       
       
       
       /**
        * @brief The task to submit for execution
        */
       template<typename DataIn,typename Similarity>
       struct Task:public parframepp::TaskBase
       {
           
           /**
            * @brief Constructor
            */
           Task(const DataIn& data,const Similarity& sim,
                const utilities::range1d<size_type>& range);
           
           
           /**
            * @brief Return the clusters computed by this task
            */
           std::vector<KMeansCluster<typename DataIn::Row> >& get_clusters(){return clusters_;}
           
           size_type get_point_counter(){return point_counter_;}
           
           utilities::range1d<size_type> get_range(){return range_;}
           
           protected:
               
            /**
              * @brief Run the task
              */
            virtual parframepp::TaskBase* run()final; 
            
            
            /**
              * @brief Pointer to the input data matrix
              */
            const DataIn* data_in_;
            
            /**
              * @brief The similarity function the task uses to compute
              * the distances from the clusters centroids
              */
            const Similarity* sim_; 
            
            /**
              * @brief the range of indices over which the task runs
              */
            utilities::range1d<size_type> range_;
                
                
            /**
              * @brief The clusters that the task manages
              */
            std::vector<KMeansCluster<typename DataIn::Row> > clusters_;
            
            /**
             * @brief Acounter that shows how many data points the task worked on
             */
            size_type point_counter_;
                      
       };
       
       std::unique_ptr<parframepp::TaskBase> main_task_;
              
    };
    
    template<typename DataIn,typename Similarity>
    void 
    Iteration<parframepp::platform_type::Type::CPUT_C11>::create_tasks(const DataIn& data,const Similarity& sim,
                                    const std::vector<utilities::range1d<size_type> >& parts){
        
        typedef Iteration<parframepp::platform_type::Type::CPUT_C11>::Task<DataIn,Similarity> task_type;
        
        parframepp::model_impl_base::task_creator<task_type>::create_tasks(parts.size()-1,*this,data,sim,parts); 
        
        for(size_type t=0; t<this->n_tasks(); ++t){
            
           auto& task_clusters = static_cast<task_type*>(this->tasks_[t])->get_clusters();
           task_clusters.resize(cntrl.k); 
           
           //utilities::range1d<size_type> range = static_cast<task_type*>(this->tasks_[t])->get_range();
           
           //std::cout<<"Task: "<<t<< " range: "<<range.begin()<<" "<<range.end()<<std::endl;
        }
        
        main_task_.reset(new task_type(data,sim,parts[parts.size()-1]));
        static_cast<task_type*>(main_task_.get())->get_clusters().resize(cntrl.k);
        //utilities::range1d<size_type> range = static_cast<task_type*>(main_task_.get())->get_range();
        //std::cout<<"Main Task range: "<<range.begin()<<" "<<range.end()<<std::endl;
    }
    
    template<typename RowType>
    void 
    Iteration<parframepp::platform_type::Type::CPUT_C11>::merge_clusters(std::vector<KMeansCluster<RowType> >& clusters,
                                                                         const std::vector<KMeansCluster<RowType> >& localclusters){
#ifdef PARML_DEBUG
        using utilities::ExeLogicError;
       std::string msg ="In Iteration<parframepp::platform_type::Type::CPUT_C11>::merge_clusters(). Incompatible sizes "+
               std::to_string(clusters.size())+" not equal to: "+std::to_string(localclusters.size());
       Assert(clusters.size()== localclusters.size(),ExeLogicError(msg));
                         
#endif
        
        for(size_type c=0; c < localclusters.size(); ++c){
            clusters[c].add_to_cluster(localclusters[c].points);
        }   
    }
    
    template<typename DataIn,typename Similarity>
    typename KMeansBase<typename DataIn::Row>::ActionType 
    Iteration<parframepp::platform_type::Type::CPUT_C11>::operator()(const DataIn& data,
                                                                     const Similarity& sim,
                                                            std::vector<KMeansCluster<typename DataIn::Row> >& clusters,
                                                            std::vector<typename DataIn::Row>& centroids){
        
#ifdef PARML_DEBUG
        using utilities::ExeLogicError;
        std::string  msg = "In Iteration<parframepp::platform_type::Type::CPUT_C11>::operator(). Tasks have not been created.";
        Assert(this->has_tasks(),ExeLogicError(msg));
#endif
        
        //clear the points the task has previously formed 
        for(size_type c=0; c< clusters.size(); ++c)
            clusters[c].points.clear();
        
        typedef typename DataIn::Row Row;
        typedef Iteration<parframepp::platform_type::Type::CPUT_C11>::Task<DataIn,Similarity> task_type;
        
        this->set_tasks_state(parframepp::TaskBase::TaskState::PENDING);
        
        //set the new centroids for the local clusters
        
        for(size_type t=0; t<this->n_tasks(); ++t){
            
           auto& task_clusters = static_cast<task_type*>(tasks_[t])->get_clusters();
           
           for(size_type c=0; c<task_clusters.size(); ++c){
               task_clusters[c].center = centroids[c];    
           }
        }
        
        
        //we assume that we have tasks so simply
        //assign schedule them for running
        parframepp::rt_cput::schedule_tasks_async(this->tasks_);
        
        auto& task_clusters = static_cast<task_type*>(main_task_.get())->get_clusters();
        
        for(size_type c=0; c<task_clusters.size(); ++c){
               task_clusters[c].center = centroids[c];      
        }
        
        main_task_->execute();
        
        //std::cout<<"Main task worked on: "<<static_cast<task_type*>(main_task_.get())->get_point_counter()<<std::endl;
        
        //merge the clusters from the main task
        merge_clusters(clusters,static_cast<task_type*>(main_task_.get())->get_clusters());
        
        /*{
            
            std::cout<<"Clusters sum after main merging"<<std::endl;
            size_type total_points = 0;
            for(size_type c=0; c<clusters.size(); ++c){
            
                total_points += clusters[c].points.size();
                std::cout<<"Cluster: "<<c<<" points: "<<clusters[c].points.size()<<std::endl;   
            }
            
            std::cout<<"total points: "<<total_points<<std::endl;
        }*/
        
        //place a barrier on the running tasks
        parframepp::task_barrier::barrier(this->tasks_);
        
        //for(size_type t=0; t<this->n_tasks(); ++t){
            
        //   std::cout<<"Task: "<<t<<" worked on: "<<static_cast<task_type*>(tasks_[t])->get_point_counter()<<std::endl;
        //}
        
        //accumulate local clusters and compute what is needed
        
        for(size_type t=0; t<this->n_tasks(); ++t){
            
           //std::cout<<"Task: "<<t<<std::endl;
           //size_type total_task_points = 0;
           auto& task_clusters = static_cast<task_type*>(tasks_[t])->get_clusters();
           //for(size_type c=0; c<task_clusters.size(); ++c){
               
            //   total_task_points += task_clusters[c].points.size();
            //   std::cout<<"Cluster: "<<c<<" points: "<<task_clusters[c].points.size()<<std::endl;
           //}
           
           //std::cout<<"Total Task points: "<<total_task_points<<std::endl;
           merge_clusters(clusters,task_clusters);
        }
        
        /*std::cout<<"Clusters sum after merging"<<std::endl;
        size_type total_points = 0;
        for(size_type c=0; c<clusters.size(); ++c){
            
            total_points += clusters[c].points.size();
           std::cout<<"Cluster: "<<c<<" points: "<<clusters[c].points.size()<<std::endl;   
        }
        
        std::cout<<"Total clustered points: "<<total_points<<std::endl;*/
        
        //check if we have an empty cluster
        //check for empty cluster
        std::pair<bool,size_type> empty_cluster_found = KMeansCluster<Row>::detect_empty_cluster(clusters); 

        if(empty_cluster_found.first && cntrl.random_restart_on_empty_cluster){

           if(cntrl.show_iterations){
              std::cout<<"\tAn empty cluster was found  restarting..."<<std::endl;
         }

          return KMeansBase<Row>::ActionType::RESTART;
        }
        else if(empty_cluster_found.first && !cntrl.continue_on_empty_cluster){

            if(cntrl.show_iterations){
               std::cout<<"\tAn empty cluster was found  breaking..."<<std::endl;
            }
            return KMeansBase<Row>::ActionType::BREAK;
        }

        //recalculate the centroids for each cluster
        //this can also be done in parallel
        for(size_type c=0; c<clusters.size(); ++c){

           clusters[c].recalculate_centroid(data);
        }

        //check for convergence
        bool converged = KMeansCluster<Row>::detect_convergence(sim,centroids,clusters,cntrl.tolerance);

        if(converged){

           if(cntrl.show_iterations){
             std::cout<<"\tKMeans converged with tolerance: "<<cntrl.tolerance<<std::endl;
           }

          return KMeansBase<Row>::ActionType::CONVERGED;   
        }

        //update the old centers
        for(size_type c=0; c<centroids.size(); ++c){
            centroids[c] = clusters[c].center; 
        }
        
        return KMeansBase<Row>::ActionType::CONTINUE;        
    }
    
    template<typename DataIn,typename Similarity>
    Iteration<parframepp::platform_type::Type::CPUT_C11>::Task<DataIn,Similarity>::Task(const DataIn& data, 
                                                                                        const Similarity& sim,
                                                                                        const utilities::range1d<size_type>& range)
    :
    parframepp::TaskBase(),
    data_in_(&data),
    sim_(&sim),
    range_(range),
    clusters_(),
    point_counter_(0)
    {}
    
    template<typename DataIn,typename Similarity>
    parframepp::TaskBase*
    Iteration<parframepp::platform_type::Type::CPUT_C11>::Task<DataIn,Similarity>::run(){
       
        point_counter_ = 0;
        
        //clear the points the task has previously formed 
        for(size_type c=0; c< clusters_.size(); ++c)
            clusters_[c].points.clear();
        
        typedef typename DataIn::Row DataPoint;

        size_type rangeb = range_.begin();
        size_type rangee = range_.end();
        
        while(rangeb != rangee){
           
             DataPoint p = data_in_->row(rangeb); 

            //cluster the data point
            KMeansCluster<DataPoint>::cluster(p,rangeb,*sim_,clusters_);
            rangeb++;
            point_counter_++;
            
        }

       return nullptr; 
    }
    
}


//template and inline methods

template<typename DataPoint>
template<typename DataIn,typename Similarity,typename Initializer>
void 
KMeans<parframepp::platform_type::Type::CPUT_C11,DataPoint>::cluster(const DataIn& data,
                                                                   const Similarity& similarity,const Initializer& init){

    size_type k = this->cntrl_.k;
    size_type rows = data.n_rows();
    size_type max_itrs = this->cntrl_.max_n_iterations;

#ifdef PARML_DEBUG
    const std::string func_msg = "In KMeans<parframepp::platform_type::Type::CPUT_C11,DataPoint>::cluster(). ";
    kmeans_detail::assert_comon(func_msg,rows,k,max_itrs);
    
    //make sure that we have partitions
    std::string msg = func_msg+" No partitions have been set.";
    using utilities::ExeLogicError;
    Assert(this->has_partitions(),ExeLogicError(msg));
#endif
    
    this->info_.nthreads = parframepp::rt_cput::n_threads();
    this->info_.n_clustering_points = data.n_rows();
    this->info_.residual = this->cntrl_.tolerance;
    
 //start timing 
 std::chrono::time_point<std::chrono::system_clock> start, end;
 start = std::chrono::system_clock::now();
 
 random_restart:
 
 
    //the old centroids
    std::vector<DataPoint> centroids;
 
    //initialize the clusters
    kmeans_detail::DataOnProcInitializer::initialize(data,init,this->cntrl_.k,this->clusters_,centroids);
 
    //the exit condition
    kmeans_detail::ExitCondition exit_cond(this->cntrl_.max_n_iterations);
    
    //the object that handles the iteration
    kmeans_detail::Iteration<parframepp::platform_type::Type::CPUT_C11> iteration(this->cntrl_);
    
    //create the tasks
    iteration.create_tasks(data,similarity,this->get_partitions());
    
    //what the algorithm should do at each iteration
    typename KMeansBase<DataPoint>::ActionType action = KMeansBase<DataPoint>::ActionType::INVALID_TYPE;
    
    while(exit_cond.check() == false){
        
        if(this->cntrl_.show_iterations){
            
            std::cout<<"\tK-means iteration: "<<exit_cond.current_itr<<std::endl;
        }
        
        action = iteration(data,similarity,this->clusters_,centroids);
        
        if(action == KMeansBase<DataPoint>::ActionType::RESTART)
            goto random_restart;
        else if(action == KMeansBase<DataPoint>::ActionType::BREAK)
            break;
        else if(action == KMeansBase<DataPoint>::ActionType::CONVERGED){
            this->info_.converged = true;
            break;   
        }
    }
    
    if(action == KMeansBase<DataPoint>::ActionType::CONVERGED){
        
        this->info_.clusters.resize(this->clusters_.size());
        for(size_type c=0; c<this->clusters_.size(); ++c){
            this->info_.clusters[c]=std::make_pair(c,this->clusters_[c].points.size());   
        }
    }
    this->info_.niterations = exit_cond.current_itr;
    end = std::chrono::system_clock::now();
    this->info_.runtime = end-start;    
}


    
    
    
}



#endif	/* CPUT_C11_KMEASN_H */


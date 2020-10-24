/* 
 * File:   knn_mpi.h
 * Author: david
 *
 * Created on October 19, 2016, 4:34 PM
 */

#ifndef KNN_MPI_H
#define	KNN_MPI_H

#include "parml/base/parml_config.h"

#ifdef PARML_HAS_MPI

#include "parml/base/parml_config.h"
#include "parml/base/parml.h"
#include "parml/instance_learning/knn_type.h"
#include "parml/instance_learning/knn_info.h"
#include "utilities/range1d.h"
#include "utilities/data_input.h"

#include "parframepp/runtime/mpi_rt_base.h"
#include "parframepp/base/parallel_indexed_object.h"
#include "parframepp/base/data_distribution_type.h"

#include <boost/scoped_array.hpp>

#include <chrono>

namespace parml
{
    
/**
 * @brief Implementation of K-nn algorithm
 * with MPI
 */   
template<>
class Knn<knn_type::Type::MPI_KNN>: public parframepp::par_indexed_object
{
  
     public:
         
         /**
         * @brief Constructor
         */
        Knn(parframepp::data_dist_type::Type t = parframepp::data_dist_type::Type::CHUNK_ON_PROC);
        
        /**
         * @brief For the given data set predict the value for the given data point
         * The actor does all the work and distinguishes between classification and regression.
         */
        template<typename DataMat,typename DataVec,typename Similarity,typename Actor>
        std::pair<typename Actor::return_type,KnnInfo>
        predict(const utilities::DataInput<DataMat,DataVec>& data,const DataVec& point,
                const Similarity& sim,Actor& actor);
        
        
    protected:
    
         /**
           * @brief When working with distributed memory, this
           * flag specifies how the data is distributed. The default
           * is that the data is only held by the master process.
        */
        parframepp::data_dist_type::Type data_dist_type_flag_;
     
};


template<typename DataMat,typename DataVec,typename Similarity,typename Actor>
std::pair<typename Actor::return_type,KnnInfo>
Knn<knn_type::Type::MPI_KNN>::predict(const utilities::DataInput<DataMat,DataVec>& data,
                                      const DataVec& point,
                                      const Similarity& sim,Actor& actor){
    
    std::chrono::time_point<std::chrono::system_clock> start, end;
    start = std::chrono::system_clock::now();
    
    size_type k = actor.n_neighbors();
    size_type nrows = data.t1.n_rows();
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
    info.nprocs = parframepp::rt_base<mpi_par>::n_processors();
    info.nthreads = parframepp::rt_base<mpi_par>::n_threads();
    info.type = knn_type::Type::MPI_KNN;
    
    size_type pid = parframepp::rt_base<mpi_par>::pid();
    size_type master_pid = parframepp::rt_base<mpi_par>::master_process_id();
    
    //the processor range 
    utilities::range1d<size_type> proc_range;
    
    if(data_dist_type_flag_ == parframepp::data_dist_type::Type::CHUNK_ON_PROC){
        
        proc_range = this->get_local_indices(pid);
    }
    else if(data_dist_type_flag_ == parframepp::data_dist_type::Type::COPY_ON_PROC){
        
        proc_range = this->get_partition(pid);    
    }
        
    //make the local computations for the actor
    actor(data,point,sim,proc_range);
        
    //now we have computed the distances. If we do classification
    //we must send the top-k class indices. if we do regression we must
    //send the top-k values
        
    typedef typename Actor::comm_chunk comm_chunk;
    boost::scoped_array<comm_chunk> proc_results;
        
    //get the result computed on the processor
    actor.get_top_k_results(proc_results);
        
    boost::scoped_array<comm_chunk> results;
        
    if(pid == master_pid){
       results.reset(new comm_chunk[k*info.nprocs]);
    }
        
        //this should not be empty!!!
        ///and every processor must send k !!!
        
         //gather at the master process
    parframepp::rt_base<mpi_par>::gather(proc_results.get(),k,results.get(),master_pid);
    
    typename Actor::return_type rslt = Actor::invalid_result();
         
    if(pid == master_pid){
       rslt = actor.get_result(results,k*info.nprocs);
    }
         
    end = std::chrono::system_clock::now();
    info.runtime = end-start;
    return std::make_pair(rslt,info);      
  }   
}
#endif
#endif	/* KNN_MPI_H */


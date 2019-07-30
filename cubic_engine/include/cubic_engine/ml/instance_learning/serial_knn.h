/* 
 * File:   serial_knn.h
 * Author: david
 *
 * Created on October 17, 2016, 8:19 PM
 */

#ifndef SERIAL_KNN_H
#define	SERIAL_KNN_H
#include "parml/base/parml_config.h"
#include "parml/base/parml.h"
#include "parml/instance_learning/knn_type.h"
#include "parml/instance_learning/knn_info.h"
#include "utilities/range1d.h"
#include "utilities/data_input.h"

#ifdef PARML_DEBUG
#include "utilities/exceptions.h"
#include <string>
#endif

#include <utility>
#include <chrono>

namespace parml
{
    
/**
 * @brief Serial implementation
 * of K-nearest neighbors algorithm
 */
template<>
class Knn<knn_type::Type::SERIAL_KNN>
{
    
    public:
        
        
        
        /**
         * @brief Constructor
         */
        Knn();
        
        
        template<typename DataMat,typename DataVec,typename Similarity,typename Actor>
        std::pair<typename Actor::return_type,KnnInfo> 
        predict(const utilities::DataInput<DataMat,DataVec>& data,
                const DataVec& point,const Similarity& sim,Actor& actor);
    
};

template<typename DataMat,typename DataVec,typename Similarity,typename Actor>
std::pair<typename Actor::return_type,KnnInfo>
Knn<knn_type::Type::SERIAL_KNN>::predict(const utilities::DataInput<DataMat,DataVec>& data,
                                         const DataVec& point,const Similarity& sim,Actor& actor){
    
    std::chrono::time_point<std::chrono::system_clock> start, end;
    start = std::chrono::system_clock::now();
    
    size_type k = actor.n_neighbors();
    size_type nrows = data.t1.n_rows();
    size_type labels_size = data.t2.size();
    
#ifdef PARML_DEBUG
    //using more neighbors than the actual rows of the input matrix is an error
    using utilities::ExeLogicError;
    std::string msg = "In Knn<knn_type::Type::SERIAL_KNN>::predict(). Number of neighbors: "+std::to_string(k)+
                            " not in [1,"+std::to_string(nrows)+")";
    Assert(((k>=1) && (k < nrows)),ExeLogicError(msg)); 
    
    msg = "In Knn<knn_type::Type::SERIAL_KNN>::predict(). Data point is empty.";
    Assert(point.size() != static_cast<size_type>(0),ExeLogicError(msg));
    
    //labels should not be empty and should be of same size
    //as the number number of training examples
    msg = "In Knn<knn_type::Type::SERIAL_KNN>::predict(). Labels size: "+
            std::to_string(labels_size)+" not equal to: "+std::to_string(nrows);
    
    Assert(labels_size == nrows,ExeLogicError(msg));
    
#endif 
    
    KnnInfo info;
    info.n_neighbors = k;
    info.n_pts_predicted = 1;
    info.nprocs = 1;
    info.nthreads = 1;
    info.type = knn_type::Type::SERIAL_KNN;
    
    //for serial knn the range is all the
    //data set
    utilities::range1d<size_type> r(0,nrows);
    
    //find the k smallest distances of
    //the given point from the given data set
    actor(data,point,sim,r);
    
    //get the result
    auto rslt = actor.get_result();
    
    end = std::chrono::system_clock::now();
    info.runtime = end-start;
    
    return std::make_pair(rslt,info);
}
   
}



#endif	/* SERIAL_KNN_H */


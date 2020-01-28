#ifndef THREADED_KNN_H
#define	THREADED_KNN_H

#include "cubic_engine/base/cubic_engine_types.h"
#include "cubic_engine/ml/instance_learning/utils/knn_control.h"
#include "cubic_engine/ml/instance_learning/utils/knn_info.h"

#include "kernel/utilities/range_1d.h"
#include "kernel/maths/matrix_utilities.h"
#include "kernel/parallel/threading/simple_task.h"
#include "kernel/utilities/range_1d.h"
#include "kernel/maths/matrix_traits.h"

#include <utility>
#include <chrono>
#include <memory>


namespace cengine
{
    

/// \brief Threaded implementation
/// of K-nearest neighbors algorithm

template<typename DataSetType, typename LabelType, typename Similarity, typename Actor>
class ThreadedKnn
{
    
public:

    /// \brief
    typedef KnnInfo output_t;

    /// \brief The return type
    typedef typename Actor::return_t return_t;
        
    /// \brief Constructor
    ThreadedKnn(const KnnControl& control);

    /// \brief Train the model
    void train(const DataSetType& data_set, const LabelType& labels);

    /// \brief Predict outcome for the given vector
    template<typename DataPoint, typename Executor, typename Options>
    std::pair<return_t, output_t> predict(const DataPoint& data,
                                          Executor& execute, const Options& option);

    /// \brief Predict outcome for the given dataset
    template<typename Executor, typename Options>
    std::pair<std::vector<return_t>, output_t> predict(const DataSetType& data, Executor& execuotor, const Options& options);

private:

     const KnnControl input_;
     const DataSetType* data_ptr_;
     const LabelType* labels_ptr_;

     /// \brief Inner class that wraps the task to be executed
     template<typename DataPoint>
     struct Task;

     /// \brief list of tasks
     std::vector<std::unique_ptr<kernel::SimpleTaskBase<typename Actor::return_t>>> tasks_;
       
};

template<typename DataSetType, typename LabelType, typename Similarity, typename Actor>
ThreadedKnn<DataSetType, LabelType, Similarity, Actor>::ThreadedKnn(const KnnControl& control)
    :
   input_(control),
   data_ptr_(nullptr),
   labels_ptr_(nullptr)
{}


template<typename DataSetType, typename LabelType, typename Similarity, typename Actor>
void
ThreadedKnn<DataSetType, LabelType, Similarity, Actor>::train(const DataSetType& data_set, const LabelType& labels){
    data_ptr_ = &data_set;
    labels_ptr_ = &labels;
}


/// \brief Inner class definition
template<typename DataSetType, typename LabelType, typename Similarity, typename Actor>
template<typename DataPoint>
struct 
ThreadedKnn<DataSetType, LabelType, Similarity, Actor>::Task: public kernel::SimpleTaskBase<typename Actor::return_t>
{
public:

   /// \brief Constructor
   Task(uint_t id, uint_t k);

   /// \brief Constructor
   Task(uint_t id, uint_t k, const DataSetType& data,
        const LabelType& labels, const DataPoint& point);

   /// \brief Reset the point to work on
   void reset_point(const DataPoint& point){point_ = &point;}

   /// \brief Reset the DataSet and labels
   void reset_data(const DataSetType& data,
                   const LabelType& labels);
                            
protected:
                
   /// \brief Implements the workings of the task
   virtual void run()override final;

   /// \brief Pointer to the input data matrix
   const DataSetType* data_;

   /// \brief The query point
   const LabelType* labels_;

   /// \brief The point to consider
   const DataPoint* point_;

   /// \brief The object that implements the knn
   /// algorithm classification or regression.
   Actor actor_;

   /// \brief Pointer to the similarity object
   Similarity sim_;

};


template<typename DataSetType, typename LabelType,
         typename Similarity, typename Actor>
template<typename DataPoint>
ThreadedKnn<DataSetType, LabelType, Similarity, Actor>::Task<DataPoint>::Task(uint_t id, uint_t k)
 :
kernel::SimpleTaskBase<typename Actor::return_t>(id),
data_(nullptr),
labels_(nullptr),
point_(nullptr),
actor_(k),
sim_()
{}

template<typename DataSetType, typename LabelType,
         typename Similarity, typename Actor>
template<typename DataPoint>
ThreadedKnn<DataSetType, LabelType, Similarity, Actor>::Task<DataPoint>::Task(uint_t id, uint_t k, const DataSetType& data,
                                                                              const LabelType& labels, const DataPoint& point)
 :
kernel::SimpleTaskBase<Null>(id),
data_(&data),
labels_(&labels),
point_(&point),
actor_(k),
sim_()
{}


template<typename DataSetType, typename LabelType, typename Similarity, typename Actor>
template<typename DataPoint>
void
ThreadedKnn<DataSetType, LabelType, Similarity, Actor>::Task<DataPoint>::reset_data(const DataSetType& data,
                                                                                     const LabelType& labels){

   data_ = &data;
   labels_ = &labels;
}

template<typename DataSetType, typename LabelType, typename Similarity, typename Actor>
template<typename DataPoint>
void
ThreadedKnn<DataSetType, LabelType, Similarity, Actor>::Task<DataPoint>::run(){
    
    actor_.resume();
    auto& range = data_->get_partition(this->get_id());
    actor_(*data_, *labels_, *point_, sim_, range);

    //get the result
    auto rslt = actor_.get_result();

    // set the result for this task
    this->result_.get_resource() = rslt;
    this->result_.validate_result();
}



template<typename DataSetType, typename LabelType, typename Similarity, typename Actor>
template<typename DataPoint, typename Executor, typename Options>
std::pair<typename ThreadedKnn<DataSetType, LabelType, Similarity, Actor>::return_t,
          typename ThreadedKnn<DataSetType, LabelType, Similarity, Actor>::output_t>
ThreadedKnn<DataSetType, LabelType, Similarity, Actor>::predict(const DataPoint& point,
                                                                Executor& executor, const Options& options){
    
    std::chrono::time_point<std::chrono::system_clock> start, end;
    start = std::chrono::system_clock::now();

    if(data_ptr_ == nullptr){
        throw std::logic_error("Dataset pointer in null");
    }

    if(labels_ptr_ == nullptr){
        throw std::logic_error("Labels pointer in null");
    }
    
    uint_t k = input_.k;
    uint_t nrows = data_ptr_->rows();
    uint_t labels_size = labels_ptr_->size();

    if(labels_size != nrows){
        throw std::logic_error("Labels size: " +
                               std::to_string(labels_size) +
                               " does not match dataset rows: " +
                               std::to_string(nrows));
    }

    if(k == 0 || k>= nrows){
        throw std::logic_error("Number of neighbors: "+std::to_string(k)+
                               " not in [1,"+std::to_string(nrows)+")");
    }

    KnnInfo info;
    info.n_neighbors = k;
    info.n_pts_predicted = 1;
    info.nprocs = 1;
    info.nthreads = executor.get_n_threads();

    typedef ThreadedKnn<DataSetType, LabelType, Similarity, Actor>::Task<DataPoint> task_t;

    if(tasks_.empty()){

        // we don't have tasks so we create them
        tasks_.reserve(executor.get_n_threads());

        for(uint_t t=0; t<executor.get_n_threads(); ++t){
            tasks_.push_back(std::make_unique<task_t>(t, k, *data_ptr_, *labels_ptr_, point));
        }

    }
    else{

        for(uint_t t=0; t<executor.get_n_threads(); ++t){
            dynamic_cast<task_t*>(tasks_[t].get())->reset_point(point);
        }
    }

    // this should block
    executor.execute(tasks_, options);

    // now reduce the result

    end = std::chrono::system_clock::now();
    info.runtime = end-start;

    typename ThreadedKnn<DataSetType, LabelType, Similarity, Actor>::return_t result;
    return {result, info};
      
}

template<typename DataSetType, typename LabelType, typename Similarity, typename Actor>
template<typename Executor, typename Options>
std::pair<std::vector<typename ThreadedKnn<DataSetType, LabelType, Similarity, Actor>::return_t>,
                      typename ThreadedKnn<DataSetType, LabelType, Similarity, Actor>::output_t>
ThreadedKnn<DataSetType, LabelType, Similarity, Actor>::predict(const DataSetType& data, Executor& executor, const Options& options){

    std::chrono::time_point<std::chrono::system_clock> start, end;
    start = std::chrono::system_clock::now();

    if(data_ptr_ == nullptr){
        throw std::logic_error("Dataset pointer in null");
    }

    if(labels_ptr_ == nullptr){
        throw std::logic_error("Labels pointer in null");
    }

    uint_t k = input_.k;
    uint_t nrows = data_ptr_->rows();
    uint_t labels_size = labels_ptr_->size();

    if(labels_size != nrows){
        throw std::logic_error("Labels size: " +
                               std::to_string(labels_size) +
                               " does not match dataset rows: " +
                               std::to_string(nrows));
    }

    if(k == 0 || k>= nrows){
        throw std::logic_error("Number of neighbors: "+std::to_string(k)+
                               " not in [1,"+std::to_string(nrows)+")");
    }

    KnnInfo info;
    info.n_neighbors = k;
    info.n_pts_predicted = 1;
    info.nprocs = 1;
    info.nthreads = executor.get_n_threads();

    typedef typename kernel::matrix_row_trait<DataSetType>::row_t row_t;
    typedef ThreadedKnn<DataSetType, LabelType, Similarity, Actor>::Task<row_t> task_t;

    if(tasks_.empty()){

        // we don't have tasks so we create them
        tasks_.reserve(executor.get_n_threads());

        for(uint_t t=0; t<executor.get_n_threads(); ++t){
            tasks_.push_back(std::make_unique<task_t>(t, k));
        }

    }

    for(uint_t t=0; t<executor.get_n_threads(); ++t){
        dynamic_cast<task_t*>(tasks_[t].get())->reset_data(*data_ptr_, *labels_ptr_);
    }

    std::vector<typename ThreadedKnn<DataSetType, LabelType, Similarity, Actor>::return_t> result(data.rows());
    for(uint_t r=0; r<data.rows(); ++r){

        auto point = kernel::matrix_row_trait<DataSetType>::get_row(data, r);

        for(uint_t t=0; t<executor.get_n_threads(); ++t){
            dynamic_cast<task_t*>(tasks_[t].get())->reset_point(point);
        }

        // this should block
        executor.execute(tasks_, options);

        //collect the results from this point
    }

    return {std::move(result), info};

}
    
}



#endif	/* CPUT_C11_KNN_H */


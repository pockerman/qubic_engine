#ifndef PARALLEL_REDUCE_H
#define PARALLEL_REDUCE_H

#include "kernel/base/types.h"
#include "kernel/base/config.h"
#include "kernel/base/exceptions.h"
#include "kernel/utilities/range_1d.h"
#include "kernel/utilities/iterator_helpers.h"
#include "kernel/parallel/threading/simple_task.h"
#include "kernel/parallel/threading/task_uitilities.h"
#include "kernel/parallel/utilities/result_holder.h"


#include <vector>
#include <memory>
#include <thread>

namespace kernel
{

namespace detail
{

    template<typename IteratorTp, typename ReductionOpTp>
    class reduce_1d_array
    {

    public:

        /// \brief Execute the reduction operation using the given partitions of the
        /// data and the given executor
        template<typename ExecutorTp, typename Options>
        void execute(const std::vector<range1d<IteratorTp>>& , ReductionOpTp& op,
                     ExecutorTp& executor, const Options& options = Null());

        /// \brief Returns true if the spawned tasks have finished
        bool tasks_finished()const{return kernel::taskutils::tasks_finished(tasks_);}

    private:

        /// \brief Struct describing the reduction task
        struct reduce_task: public SimpleTaskBase<typename ReductionOpTp::value_type>
        {
            typedef typename ReductionOpTp::value_type result_type;

            /// \brief Constructor
            reduce_task(const range1d<IteratorTp>& range);

            //result_type get_result()const{return local_rslt_;}
            //result_type local_rslt_;
            range1d<IteratorTp> range_;

        protected:

            /// \brief Override base class run method
            virtual void run()override final;
        };

        /// \brief The tasks to be scheduled
        std::vector<std::unique_ptr<reduce_task>> tasks_;
    };

    template<typename IteratorTp, typename ReductionOpTp>
    reduce_1d_array<IteratorTp,ReductionOpTp>::reduce_task::reduce_task(const range1d<IteratorTp>& range)
        :
      SimpleTaskBase<typename ReductionOpTp::value_type>(),
      range_(range)
    {}

    template<typename IteratorTp, typename ReductionOpTp>
    void
    reduce_1d_array<IteratorTp, ReductionOpTp>::reduce_task::run(){

        for(const auto& item : range_){

            ReductionOpTp::local_join(iterator_value_accessor<IteratorTp>::get(item),
                                      this->result_.get_resource());
        }
    }

    template<typename IteratorTp, typename ReductionOpTp>
    template<typename ExecutorTp, typename Options>
    void
    reduce_1d_array<IteratorTp, ReductionOpTp>::execute(const std::vector<range1d<IteratorTp>>& partitions , ReductionOpTp& op,
                                                        ExecutorTp& executor, const Options& options){

        // Currently don't try to do anything smart if the
        // there is  mismatch
        if( partitions.size() != executor.get_n_threads()){
            throw std::runtime_error("Incompatible number of partitions with number of threads");
        }

        typedef reduce_1d_array<IteratorTp, ReductionOpTp>::reduce_task task_type;
        tasks_.reserve(executor.get_n_threads());

        //spawn the tasks
        for(uint_t t = 0; t < executor.get_n_threads(); ++t){
            tasks_.push_back(std::make_unique<task_type>(partitions[t]));    
        }

        // this should block
        executor.execute(tasks_, options); //add_task(*(tasks_[t].get()));

        // if the tasks have not finished yet
        // then the calling thread waits here
        /*while(!tasks_finished()){
           std::this_thread::yield();
        }*/

        //validate the result
        op.validate_result();

        for(uint_t t=0; t < tasks_.size(); ++t){

            // if we reached here but for some reason the
            // task has not finished properly invalidate the result
           if(tasks_[t]->get_state() != TaskBase::TaskState::FINISHED){
               op.invalidate_result(false);
           }
           else{
               op.join(static_cast<task_type*>(tasks_[t].get())->get_result().get_resource());
           }
        }
    }
}

template<typename IteratorTp, typename ReductionOpTp, typename ExecutorTp>
void reduce_array(const std::vector<range1d<IteratorTp>>& partitions, ReductionOpTp& op, ExecutorTp& executor){

    detail::reduce_1d_array<IteratorTp, ReductionOpTp> reduce;
    reduce.execute(partitions, op, executor, Null());
}

template<typename RangeTp, typename ReduceOpTp, typename ExecutorTp>
ResultHolder<typename ReduceOpTp::result_type>
parallel_reduce(const RangeTp& range, const ReduceOpTp& reduction_op, ExecutorTp& executor){

    if(!range.has_partitions()){
        throw InvalidPartitionedObject("The given range does not have partitions");
    }

    if(range.n_partitions() != executor.n_processing_elements()){
        throw InvalidPartitionedObject("Invalid number of partitions: "+
                                       std::to_string(range.n_partitions())+" should be: "+
                                       std::to_string(executor.n_processing_elements()));
    }
}


}


#endif // REDUCE_H

#ifndef REDUCE_H
#define REDUCE_H

#include "parframe/base/types.h"
#include "parframe/executors/simple_task.h"
#include "parframe/data_structs/range_1d.h"
#include "parframe/utilities/iterator_helpers.h"
#include "parframe/executors/task_uitilities.h"

#include <vector>
#include <memory>
namespace parframe
{

namespace detail
{

    class reduce_1d_array
    {

    public:

        /// \brief Execute the reduction operation using the given partitions of the
        /// data and the given executor
        template<typename IteratorTp, typename ReductionOpTp, typename ExecutorTp>
        void execute(const std::vector<range1d<IteratorTp>>& , ReductionOpTp& op, ExecutorTp& executor);

        /// \brief Returns true if the spawned tasks have finished
        bool tasks_finished()const{return taskutils::tasks_finished(tasks_);}

    private:

        template<typename IteratorTp, typename ReductionOpTp>
        struct reduce_task: public SimpleTaskBase
        {
            typedef typename ReductionOpTp::result_type result_type;

            /// \brief Constructor
            reduce_task(const range1d<IteratorTp>& range);

            result_type get_result()const{return local_rslt_;}
            result_type local_rslt_;
            range1d<IteratorTp> range_;

        protected:

            /// \brief Overrif base class run method
            virtual void run()override final;
        };

        /// \brief The tasks to be scheduled
        std::vector<std::unique_ptr<SimpleTaskBase>> tasks_;
    };

    template<typename IteratorTp, typename ReductionOpTp>
    reduce_1d_array::reduce_task<IteratorTp,ReductionOpTp>::reduce_task(const range1d<IteratorTp>& range)
        :
      local_rslt_(),
      range_(range)
    {}

    template<typename IteratorTp, typename ReductionOpTp>
    void
    reduce_1d_array::reduce_task<IteratorTp,ReductionOpTp>::run(){

        for(const auto& item : range_){
            ReductionOpTp::local_join(iterator_value_accessor<IteratorTp>::get(item), local_rslt_);
        }
    }

    template<typename IteratorTp, typename ReductionOpTp, typename ExecutorTp>
    void
    reduce_1d_array::execute(const std::vector<range1d<IteratorTp>>& partitions , ReductionOpTp& op, ExecutorTp& executor){

        // Currently don't try to do anything smart if the
        // there is  mismatch
        if( partitions.size() != executor.get_n_threads()){
            throw std::runtime_error("Incompatibel number of partitions with number of threads");
        }

        typedef reduce_1d_array::reduce_task<IteratorTp, ReductionOpTp> task_type;
        tasks_.reserve(executor.get_n_threads());

        //spawn the tasks
        for(uint_t t = 0; t < executor.get_n_threads(); ++t){
            tasks_.push_back(std::make_unique<task_type>(partitions[t]));
        }

        // join the local retsults
        // TODO: Perhaps we should hide this joining into a class
        std::vector<bool> task_touched(tasks_.size(), false);

        // This is O(n_tasks^2). We need
        // a better way altogether here
        while(!tasks_finished()){
            for(uint_t t=0; t < tasks_.size(); ++t){
                if( !task_touched[t]){
                    op.join(static_cast<task_type*>(tasks_[t].get())->get_result());
                    task_touched[t] = true;
                }
            }
        }

        //finally validate the result
        op.validate_result();
    }

}

template<typename IteratorTp, typename ReductionOpTp, typename ExecutorTp>
void reduce_array(const std::vector<range1d<IteratorTp>>& partitions, ReductionOpTp& op, ExecutorTp& executor){

    detail::reduce_1d_array reduce;
    reduce.execute(partitions, op, executor);
}


}


#endif // REDUCE_H

#ifndef REDUCE_H
#define REDUCE_H

#include "parframe/base/types.h"
#include "parframe/executors/simple_task.h"
#include "parframe/data_structs/range_1d.h"

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

    private:

        template<typename IteratorTp, typename ReductionOpTp>
        struct reduce_task: public SimpleTaskBase
        {
            typedef typename ReductionOpTp::result_type result_type;

            result_type get_result()const{return local_rslt_;}
            result_type local_rslt_;

        };


        /// A vector of Jacobi tasks to be executed and synchronized
        std::vector<std::unique_ptr<SimpleTaskBase>> tasks_;
    };

    template<typename IteratorTp, typename ReductionOpTp, typename ExecutorTp>
    void
    reduce_1d_array::execute(const std::vector<range1d<IteratorTp>>& , ReductionOpTp& op, ExecutorTp& executor){

        typedef reduce_1d_array::reduce_task<IteratorTp, ReductionOpTp> task_type;

        for(uint_t t=0; t<tasks_.size(); ++t){

           op.join(static_cast<task_type>(tasks_[t])->get_result());
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

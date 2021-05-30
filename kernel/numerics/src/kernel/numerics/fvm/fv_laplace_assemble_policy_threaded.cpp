#include "kernel/base/config.h"

#ifdef USE_FVM

#include "kernel/numerics/fvm/fv_laplace_assemble_policy_threaded.h"
#include "kernel/parallel/threading/thread_pool.h"
#include "kernel/parallel/threading/openmp_executor.h"
#include <exception>

namespace kernel{
namespace numerics{

template<int dim, typename Executor>

FVLaplaceAssemblyPolicyThreaded<dim, Executor>::FVLaplaceAssemblyPolicyThreaded()
    :
    fv_grads_(),
    dof_manager_(nullptr),
    boundary_func_(nullptr),
    rhs_func_(nullptr),
    volume_func_(nullptr),
    m_ptr_(nullptr),
    tasks_(),
    executor_(nullptr)
{}


#ifdef USE_TRILINOS

template<int dim, typename Executor>
void
FVLaplaceAssemblyPolicyThreaded<dim, Executor>::assemble(TrilinosEpetraMatrix& mat, TrilinosEpetraVector& x, TrilinosEpetraVector& b ){


    if(m_ptr_ == nullptr){
        throw std::logic_error("Mesh pointer is not set");
    }

    if(executor_ == nullptr){
        throw std::logic_error("Executor is not set");
    }

    if(tasks_.empty()){

        typedef FVLaplaceAssemblyPolicyThreaded<dim, Executor>::AssembleTask<TrilinosEpetraMatrix, TrilinosEpetraVector> task_t;

        tasks_.reserve(executor_->n_processing_elements());

        for(uint_t t=0; t<executor_->n_processing_elements(); ++t){
            tasks_.push_back(std::make_unique<task_t>(t, mat, b, x, fv_grads_, *dof_manager_,
                                                      *m_ptr_, boundary_func_, rhs_func_, volume_func_));
        }

    }
    else{

        for(uint_t t=0; t<tasks_.size(); ++t){
            tasks_[t]->reschedule();
        }

    }

    /// this should block
    executor_->execute(tasks_, typename Executor::default_options_t());

}
#endif

template class FVLaplaceAssemblyPolicyThreaded<1, ThreadPool>;
template class FVLaplaceAssemblyPolicyThreaded<2, ThreadPool>;
//template class FVLaplaceAssemblyPolicyThreaded<3, ThreadPool>;

template class FVLaplaceAssemblyPolicyThreaded<1, OMPExecutor>;
template class FVLaplaceAssemblyPolicyThreaded<2, OMPExecutor>;
//template class FVLaplaceAssemblyPolicyThreaded<3, OMPExecutor>;

}
}

#endif

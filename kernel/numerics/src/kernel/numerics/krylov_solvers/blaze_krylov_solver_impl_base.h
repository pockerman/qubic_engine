#ifndef BLAZE_KRYLOV_SOLVER_IMPL_BASE_H
#define BLAZE_KRYLOV_SOLVER_IMPL_BASE_H

#include "kernel/base/types.h"
#include "kernel/numerics/krylov_solvers/krylov_solver_output.h"
#include "kernel/numerics/krylov_solvers/krylov_solver_data.h"
#include "kernel/utilities/iterative_algorithm_controller.h"

#include <boost/noncopyable.hpp>

namespace kernel {
namespace numerics {

class BlazeKrylovSolverImplBase: private boost::noncopyable
{
public:

    /// \brief The output the solver returns when solving
    /// the system
    typedef KrylovSolverResult output_t;

    /// \brief Destructor
    virtual ~BlazeKrylovSolverImplBase()
    {}

    /// \brief Solve the given system
    virtual output_t solve(const SparseMatrix<real_t>& A,
                            DynVec<real_t>& x, const DynVec<real_t>& b)=0;

protected:


    /// \brief Constructor
    BlazeKrylovSolverImplBase(const KrylovSolverData& data)
        :
        controller_(data.n_iterations, data.tolerance),
        data_(data)
    {}


    /// \brief The controller of the solver
    IterativeAlgorithmController controller_;

    /// \brief Data used by the solver
    KrylovSolverData data_;

};

}

}

#endif // BLAZE_KRYLOV_SOLVER_IMPL_BASE_H

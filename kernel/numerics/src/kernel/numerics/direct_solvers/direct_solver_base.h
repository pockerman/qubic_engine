#ifndef DIRECT_SOLVER_BASE_H
#define DIRECT_SOLVER_BASE_H

#include "kernel/numerics/solvers/solver_base.h"
#include "kernel/numerics/solvers/solver_type.h"
#include "kernel/numerics/solvers/solver_package_type.h"

namespace kernel {
namespace maths{
namespace solvers{

///
/// \brief The DirectSolverBase class. Base class for direct solvers
///
template<typename MatTp, typename VecTp>
class DirectSolverBase: public SolverBase<MatTp, VecTp>
{
public:

    ///
    /// \brief matrix_t The matrix type the solver is using
    ///
    typedef typename SolverBase<MatTp, VecTp>::matrix_t matrix_t;

    ///
    /// \brief vector_t The vector type the solver is using
    ///
    typedef typename SolverBase<MatTp, VecTp>::vector_t vector_t;

protected:

    ///
    /// \brief DirectSolverBase Constructor
    ///
    DirectSolverBase(SolverPackageType ptype);

};

template<typename MatTp, typename VecTp>
DirectSolverBase<MatTp,VecTp>::DirectSolverBase(SolverPackageType ptype)
    :
      SolverBase<MatTp, VecTp> (SolverType::DIRECT, ptype)
{}


}
}
}

#endif // DIRECT_SOLVER_BASE_H

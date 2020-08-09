#ifndef SOLVER_BASE_H
#define SOLVER_BASE_H

#include "kernel/base/types.h"
#include "kernel/maths/solvers/solver_type.h"
#include "kernel/maths/solvers/solver_package_type.h"

#include <boost/noncopyable.hpp>

namespace kernel {
namespace maths{
namespace solvers {

///
/// \brief Base class to provide a common interface
/// to direct and iterative solvers when solving the
/// linear system Ax = b
///
template <typename MatTp, typename VecTp>
class SolverBase: private boost::noncopyable
{

public:

    ///
    /// \brief matrix_t The matrix type the solver is using
    ///
    typedef MatTp matrix_t;

    ///
    /// \brief vector_t The vector type the solver is using
    ///
    typedef VecTp vector_t;

    ///
    /// \brief ~SolverBase. Destructor
    ///
    virtual ~SolverBase(){}

    ///
    /// \brief solve. Solve the system Ax=b
    ///
    virtual void solve(matrix_t& A, vector_t& x, vector_t& b )=0;

    ///
    /// \brief type Returns the type of the solver
    ///
    SolverType type()const{return type_;}

    ///
    /// \brief ptype Return the package type of the solver
    ///
    SolverPackageType ptype()const{return ptype_;}

protected:

    ///
    /// \brief SolverBase. Constructor
    ///
    SolverBase(SolverType type, SolverPackageType ptype);

private:

    ///
    /// \brief type_. The type of the solver
    ///
    SolverType type_;

    ///
    /// \brief ptype_ The package type of the solver
    ///
    SolverPackageType ptype_;

};

template <typename MatTp, typename VecTp>
SolverBase<MatTp, VecTp>::SolverBase(SolverType type, SolverPackageType ptype)
    :
      type_(type),
      ptype_(ptype)
{}

}
}

}

#endif // SOLVER_BASE_H

#ifndef BLAZE_GMRES_IMPL_H
#define BLAZE_GMRES_IMPL_H

#include "kernel/numerics/krylov_solvers/blaze_krylov_solver_impl_base.h"
#include "kernel/numerics/krylov_solvers/krylov_solver_data.h"


namespace kernel{
namespace numerics{

struct GMRESData
{
    uint_t n_tmp_vectors;
};

/// \brief Implements GMRES(m)
class BlazeGMRES: public BlazeKrylovSolverImplBase
{
public:

    /// \brief The output the solver returns when solving
    /// the system
    typedef BlazeKrylovSolverImplBase::output_t output_t;

    /// \brief Constructor
    BlazeGMRES(const KrylovSolverData& data);

    /// \brief Solve the given system
    virtual output_t solve(const SparseMatrix<real_t>& A,
                            DynVec<real_t>& x, const DynVec<real_t>& b)override final;


private:

    GMRESData gmres_data_;

//    output_t solve(const SparseMatrix<real_t>& A,
//                   DynVec<real_t>& x, const DynVec<real_t>& b){}




};


}
}

#endif // BLAZE_GMRES_IMPL_H

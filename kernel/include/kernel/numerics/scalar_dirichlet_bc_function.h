#ifndef SCALAR_DIRICHLET_BC_FUNCTION_H
#define SCALAR_DIRICHLET_BC_FUNCTION_H

#include "kernel/base/types.h"
#include "kernel/numerics/boundary_function_base.h"

namespace kernel {
namespace numerics {

template<int dim>
class ScalarDirichletBCFunc: public BoundaryFunctionBase<dim>
{
public:

    typedef typename BoundaryFunctionBase<dim>::output_t output_t;

    /// \brief Constructor
    ScalarDirichletBCFunc(real_t val);

    /// \brief Constructor. Initialize all the boundaries
    /// to have Dirichlet condition
    ScalarDirichletBCFunc(real_t val, uint_t n_boundaries);

    /// \brief Returns the value of the function
    virtual output_t value(const GeomPoint<dim>&  /*input*/)const override{return value_;}

    /// \brief Returns the gradients of the function
    virtual DynVec<real_t> gradients(const GeomPoint<dim>&  /*input*/)const override{return DynVec<real_t>();}

private:

    /// \brief The BC value
    real_t value_;
};

}

}

#endif // SCALAR_DIRICHLET_BC_FUNCTION_H

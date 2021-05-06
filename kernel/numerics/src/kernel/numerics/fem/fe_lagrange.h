#ifndef FE_LAGRANGE_BASE_H
#define FE_LAGRANGE_BASE_H

#include "kernel/base/config.h"

#ifdef USE_FEM

#include "kernel/base/types.h"
#include "kernel/geometry/geom_point.h"
#include "kernel/discretization/element.h"
#include "kernel/numerics/fem/finite_element_base.h"
#include "kernel/numerics/fem/fe_enum_types.h"

#include <memory>

namespace kernel{
namespace numerics {
namespace fem {

template<int dim>
class FELagrange: public FiniteElementBase<dim>
{
public:

    ///
    /// \brief FELagrange Constructor
    ///
    FELagrange(const FE_Type type);

    ///
    /// \brief phi_i_shape_value Brief returns the value of the
    /// i-th shape function at the given point
    /// \param p
    /// \return
    ///
    virtual real_t phi_i_shape_value(uint_t i, const GeomPoint<dim>& p)const override {return fe_impl_->phi_i_shape_value(i, p);}

    ///
    /// \brief phi_i_shape_value Brief returns the gradient values of the
    /// i-th shape function at the given point
    /// \param p
    /// \return
    ///
    virtual DynVec<real_t> phi_i_shape_grad(uint_t i, const GeomPoint<dim>& p)const override{return fe_impl_->phi_i_shape_grad(i, p);}

    ///
    /// \brief n_shape_functions. Returns the number of shape functions
    ///
    virtual uint_t n_shape_functions()const override{return fe_impl_->n_shape_functions();};

private:

    ///
    /// \brief fe_impl_ The actual fe implementation
    ///
    std::unique_ptr<FiniteElementBase<dim>> fe_impl_;

};


}
}
}

#endif
#endif // FE_LAGRANGE_BASE_H

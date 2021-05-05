#ifndef FINITE_ELEMENT_BASE_H
#define FINITE_ELEMENT_BASE_H

#include "kernel/base/config.h"

#ifdef USE_FEM

#include "kernel/base/types.h"
#include "kernel/geometry/geom_point.h"
#include "kernel/discretization/element.h"
#include "kernel/numerics/fem/fe_enum_types.h"

namespace kernel{
namespace numerics {
namespace fem {

///
/// \brief The class FiniteElementBase. Base class for modeling
/// finite elements
///
template<int dim>
class FiniteElementBase{

public:

    ///
    /// \brief ~FiniteElementBase. Destructor
    ///
    virtual ~FiniteElementBase()=default;

    ///
    /// \brief phi_i_shape_value Brief returns the value of the
    /// i-th shape function at the given point
    /// \param p
    /// \return
    ///
    virtual real_t phi_i_shape_value(uint_t i, const GeomPoint<dim>& p)const=0;

    ///
    /// \brief phi_i_shape_value Brief returns the gradient values of the
    /// i-th shape function at the given point
    /// \param p
    /// \return
    ///
    virtual DynVec<real_t> phi_i_shape_grad(uint_t i, const GeomPoint<dim>& p)const=0;

    ///
    /// \brief n_shape_functions. Returns the number of shape functions
    ///
    virtual uint_t n_shape_functions()const = 0;

    ///
    /// \brief get_type
    /// \return
    ///
    FE_Type get_type()const{return fe_type_;}

protected:

    ///
    /// \brief FiniteElementBase. Constructor
    /// \param element
    ///
    FiniteElementBase(const FE_Type type);

    ///
    /// \brief fe_type_
    ///
    FE_Type fe_type_;

    ///
    /// \brief element_ptr_
    ///
    const Element<dim>* element_ptr_;

};

}

}
}
#endif

#endif // FINITE_ELEMENT_BASE_H

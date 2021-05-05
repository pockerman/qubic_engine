#include "kernel/base/config.h"

#ifdef USE_FEM
#include "kernel/numerics/fem/fe_lagrange_edge.h"
#include "kernel/numerics/fem/fe_enum_types.h"

#ifdef KERNEL_DEBUG
      #include<cassert>
      #include <stdexcept>
#endif

namespace kernel {
namespace numerics {
namespace fem {

template<int dim>
struct FELagrangeEdge<dim>::FEEdgeImpl
{

    ///
    /// \brief phi_i_edge2_shape_value
    /// \param i
    /// \param p
    /// \return
    ///
    static real_t phi_i_edge2_shape_value(uint_t i, const GeomPoint<dim>& p) noexcept;

    ///
    /// \brief phi_i_edge3_shape_value
    /// \param i
    /// \param p
    /// \return
    ///
    static real_t phi_i_edge3_shape_value(uint_t i, const GeomPoint<dim>& p) noexcept;

    ///
    /// \brief phi_i_edge4_shape_value
    /// \param i
    /// \param p
    /// \return
    ///
    static real_t phi_i_edge4_shape_value(uint_t i, const GeomPoint<dim>& p) noexcept;

    ///
    /// \brief phi_i_shape_value Brief returns the value of the
    /// i-th shape function at the given point
    /// \param p
    /// \return
    ///
    static real_t phi_i_shape_value(const FE_Type type, uint_t i, const GeomPoint<dim>& p);

    ///
    /// \brief phi_i_shape_value Brief returns the gradient values of the
    /// i-th shape function at the given point
    /// \param p
    /// \return
    ///
    static DynVec<real_t> phi_i_shape_grad(const FE_Type type, uint_t i, const GeomPoint<dim>& p);

    ///
    /// \brief n_shape_functions. Returns the number of shape functions
    ///
    static uint_t n_shape_functions(const FE_Type type);

};

template<int dim>
real_t
FELagrangeEdge<dim>::FEEdgeImpl::phi_i_edge2_shape_value(uint_t i, const GeomPoint<dim>& p) noexcept{

    auto xi = p[0];
    return i == 0 ? 0.5*(1.0 - xi) : 0.5*(1.0 + xi);

}

template<int dim>
real_t
FELagrangeEdge<dim>::FEEdgeImpl::phi_i_edge3_shape_value(uint_t i, const GeomPoint<dim>& p) noexcept{

    auto xi = p[0];
    return i == 0 ? -0.5*xi*(1.0 - xi) : i == 1 ? 0.5*xi*(1.0 + xi): (1.0 - xi)*(1.0 + xi);

}

template<int dim>
real_t
FELagrangeEdge<dim>::FEEdgeImpl::phi_i_edge4_shape_value(uint_t i, const GeomPoint<dim>& p) noexcept{


    auto xi = p[0];
    auto shape_val = 0.0;
    if(i == 0){
        shape_val = -9.0/16.0 * (1 - xi)*(0.333333 + xi)*(0.33333 - xi);
    }
    else if( i == 1){
        shape_val = -9.0/16.0 * (0.333333 + xi)*(0.333333 - xi)*(1.0 + xi);
    }
    else if (i == 2) {
        shape_val = 27.0/16.0 * (1. - xi)*(1.0 + xi)*(0.33333 - xi);
    }
    else if (i == 3) {
        shape_val = 27.0/16.0 * (1. - xi)*(1.0 + xi)*(0.33333 + xi);
    }

    return shape_val;

}

template<int dim>
real_t
FELagrangeEdge<dim>::FEEdgeImpl::phi_i_shape_value(const FE_Type type, uint_t i, const GeomPoint<dim>& p){

#ifdef KERNEL_DEBUG
         assert(i < n_shape_functions(type));
#endif

         auto shape_val = 0.0;
         switch(type){
            case FE_Type::FE_E2:{
                shape_val = FELagrangeEdge<dim>::FEEdgeImpl::phi_i_edge2_shape_value(i, p);
            }
            case FE_Type::FE_E3:{
                shape_val = FELagrangeEdge<dim>::FEEdgeImpl::phi_i_edge3_shape_value(i, p);
            }
            case FE_Type::FE_E4:{
                shape_val = FELagrangeEdge<dim>::FEEdgeImpl::phi_i_edge4_shape_value(i, p);
            }
           default:{

#ifdef KERNEL_DEBUG
         assert(false && "Invalid FE_Type for Lagrange Edge");
#endif

           }

         }

         return shape_val;
}

template<int dim>
DynVec<real_t>
FELagrangeEdge<dim>::FEEdgeImpl::phi_i_shape_grad(const FE_Type type, uint_t i, const GeomPoint<dim>& p){

    return DynVec<real_t>();

}

template<int dim>
uint_t
FELagrangeEdge<dim>::FEEdgeImpl::n_shape_functions(const FE_Type type){

    uint_t n_shapes = 0;
    switch(type){
        case FE_Type::FE_E2:{
            n_shapes = 2;
            break;
        }
        case FE_Type::FE_E3:{
            n_shapes = 3;
            break;
        }
        case FE_Type::FE_E4:{
            n_shapes = 4;
            break;
        }
        default:{

#ifdef KERNEL_DEBUG
         assert(false);
#endif

        }

    }

    return n_shapes;
}


template<int dim>
FELagrangeEdge<dim>::FELagrangeEdge(const FE_Type type)
    :
      FiniteElementBase<dim> (type),
      impl_(std::make_unique<FELagrangeEdge<dim>::FEEdgeImpl>())
{
#ifdef KERNEL_DEBUG
    if( (type != FE_Type::FE_E2) && (type != FE_Type::FE_E3) && (type != FE_Type::FE_E4)){
        throw std::logic_error("FE_Type should be in [FE_E2, FE_E3, FE_E4]");
    }
#endif
}

template<int dim>
FELagrangeEdge<dim>::~FELagrangeEdge()
{}

template<int dim>
real_t
FELagrangeEdge<dim>::phi_i_shape_value(uint_t i, const GeomPoint<dim>& p)const{
    return impl_->phi_i_shape_value(this->get_type(), i, p);
}

template<int dim>
DynVec<real_t>
FELagrangeEdge<dim>::phi_i_shape_grad(uint_t i, const GeomPoint<dim>& p)const{
    return impl_->phi_i_shape_grad(this->get_type(), i, p);
}

template<int dim>
uint_t
FELagrangeEdge<dim>::n_shape_functions()const{
    return impl_->n_shape_functions(this->get_type());
}


template class FELagrangeEdge<1>;
template class FELagrangeEdge<2>;
template class FELagrangeEdge<3>;

}
}
}

#endif

#ifndef FE_LAGRANGE_EDGE_H
#define FE_LAGRANGE_EDGE_H

#include "kernel/base/config.h"

#ifdef USE_FEM

#include "kernel/numerics/fem/finite_element_base.h"
#include <memory>

namespace kernel {
namespace numerics {
namespace fem {

template<int dim>
class FELagrangeEdge: public FiniteElementBase<dim>
{
public:

    ///
    /// \brief FELagrangeEdge Constructor
    /// \param type
    ///
    FELagrangeEdge(const FE_Type type);

    ///
    /// \brief Destructor
    ///
    ~FELagrangeEdge();



    ///
    /// \brief phi_i_shape_value Brief returns the value of the
    /// i-th shape function at the given point
    /// \param p
    /// \return
    ///
    virtual real_t phi_i_shape_value(uint_t i, const GeomPoint<dim>& p)const override final;

    ///
    /// \brief phi_i_shape_value Brief returns the gradient values of the
    /// i-th shape function at the given point
    /// \param p
    /// \return
    ///
    virtual DynVec<real_t> phi_i_shape_grad(uint_t i, const GeomPoint<dim>& p)const override final;

    ///
    /// \brief n_shape_functions. Returns the number of shape functions
    ///
    virtual uint_t n_shape_functions()const override final;

private:

    ///
    /// \brief The actual implementation
    ///
    struct FEEdgeImpl;

    ///
    /// \brief impl_ Pointer to implementation
    ///
    std::unique_ptr<FEEdgeImpl> impl_;

};

}

}

}
#endif
#endif // FE_LAGRANGE_EDGE_H

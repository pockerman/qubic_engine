#ifndef BOUNDARY_FUNCTION_BASE_H
#define BOUNDARY_FUNCTION_BASE_H

#include "kernel/base/types.h"
#include "kernel/maths/functions/function_base.h"
#include "kernel/geometry/geom_point.h"
#include "kernel/numerics/boundary_conditions_type.h"

#include <unordered_map>
namespace kernel{
namespace numerics {

template<int dim>
class BoundaryFunctionBase: public FunctionBase<real_t, GeomPoint<dim>>
{
public:

    typedef GeomPoint<dim> input_t;
    typedef typename FunctionBase<real_t, GeomPoint<dim>>::output_t output_t;

    /// \brief Constructor
    BoundaryFunctionBase();

    /// \brief Returns the value of the function
    virtual output_t value(const GeomPoint<dim>&  input)const=0;

    /// \brief Returns the gradients of the function
    virtual DynVec<real_t> gradients(const GeomPoint<dim>&  input)const=0;

    /// \brief Returns the number of coefficients
    virtual uint_t n_coeffs()const{return 0;}

    /// \brief Set the boundary condition type
    void set_bc_type(uint_t bidx, BCType type);

    /// \brief clear the boundary conditions this object holds
    void clear_bc_types(){boundary_conditions_.clear();}

    /// \brief Return the boundary condition type on the given boundary
    BCType bc_type(uint_t n_boundary)const;

    /// \brief How many boundaries we have. This is equal to the size of
    uint_t n_boundaries()const{return boundary_conditions_.size();}

    /// \brief Get the indicators of the boundaries where the specified
    ///  boundary condition is used
    std::vector<uint_t> get_boundaries(BCType type)const;

    /// \brief Return true iff the function has been assigned BCType type
    bool has_condition(BCType type)const{return !get_boundaries(type).empty();}

protected:

    /// \brief map that holds the boundary condition type
    /// at the given boundary indicator
    std::unordered_map<uint_t, BCType> boundary_conditions_;
};

}
}

#endif // BOUNDARY_FUNCTION_BASE_H

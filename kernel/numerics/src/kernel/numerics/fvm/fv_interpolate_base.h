#ifndef FV_INTERPOLATE_BASE_H
#define FV_INTERPOLATE_BASE_H

#include "kernel/base/config.h"

#ifdef USE_FVM

#include "kernel/base/types.h"
#include "kernel/numerics/fvm/fv_interpolation_types.h"

#include <map>

namespace kernel {
namespace numerics{

/// forward declarations
template<int dim> class Element;

template<int dim>
class FVInterpolateBase
{
public:

    /// \brief destructor
    virtual ~FVInterpolateBase();

    /// \brief Compute the fluxes for the given element
    virtual void compute_fluxes(const Element<dim>& elem, std::vector<real_t>& values)const=0;

    /// \brief Returns the matrix contibutions
    /// that should be used on the given element
    virtual void compute_matrix_contributions(const Element<dim>& element, std::map<uint_t, real_t>&)const=0;

    /// \brief Returns the type of the approximation
    FVInterpolationType type()const{return type_;}

protected:


    /// \brief Constructor
    FVInterpolateBase(FVInterpolationType type);

    /// \brief The type of the scheme
    FVInterpolationType type_;

};

}

}

#endif
#endif // FV_INTERPOLATE_BASE_H

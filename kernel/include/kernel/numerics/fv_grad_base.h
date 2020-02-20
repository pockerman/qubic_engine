#ifndef FV_GRAD_BASE_H
#define FV_GRAD_BASE_H

#include "kernel/base/types.h"
#include "kernel/numerics/fv_grad_types.h"
#include <vector>

namespace kernel{
namespace numerics{

/// forward declarations
template<int dim> class Element;

/// \brief Base class for deriving rules for
/// gradient approximations
template<int dim>
class FVGradBase
{

public:

    /// \brief destructor
    virtual ~FVGradBase();

    /// \brief Compute the gradients for the given element
    virtual void compute_gradients(const Element<dim>& elem, std::vector<real_t>& values)const=0;

    /// \brief Returns true if the a approximation
    /// uses some sort of correction
    virtual bool is_corrected()const=0;

    /// \brief Returns the type of the approximation
    FVGradType type()const{return type_;}

protected:

    /// \brief Constructor
    FVGradBase(FVGradType type);

    /// \brief the type of the gradient approxiamtion
    FVGradType type_;

};
}
}

#endif // FV_GRAD_BASE_H

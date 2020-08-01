#ifndef ADMM_H
#define ADMM_H

#include "kernel/base/types.h"

namespace kernel{
namespace maths {
namespace opt{

///
/// \brief The ADMM class. Implements the ADMM algorithm for solving the
/// convex quadratic problem
/// \f[ \text{minimize}~~ \frac{1}{2}x^TPx +q^Tx \f]
///
/// subject to
///
/// \f[ l \leq Ax \leq u \f]
///
class ADMM
{

public:

    ///
    /// \brief ADMM Constructor
    ///
    ADMM();



private:

    ///
    /// \brief rho_. The step size of the algorithm
    ///
    real_t rho_;

};

}
}
}

#endif // ADMM_H

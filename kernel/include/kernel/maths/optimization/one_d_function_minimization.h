#ifndef ONE_D_FUNCTION_MINIMIZATION_H
#define ONE_D_FUNCTION_MINIMIZATION_H

#include "kernel/base/types.h"
#include "kernel/utilities/common_uitls.h"

namespace kernel {
namespace maths {
namespace opt {

#define SHFT(a, b, c, d) (a)=(b); (b)=(c); (c)=(d);
constexpr real_t GOLD = 1.618034;

template<typename FunctionTp>
void min_bracket(real_t ax, real_t bx, real_t cx,
                 const FunctionTp& function){

    auto fa = function(ax);
    auto fb = function(bx);
    real_t dum;

    // Switch roles of a and b so that we can go
    // downhill in the direction from a to b.

    if(fb > fa){
         utils::shift(dum, ax, bx, dum);
         utils::shift(dum, fb, fa, dum);
    }

    // guess for c
    cx = bx + GOLD*(bx-ax);
    auto fc=function(cx);


}

}

}

}

#endif // 1D_FUNCTION_MINIMIZATION_H

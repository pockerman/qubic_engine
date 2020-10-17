#ifndef ONE_D_FUNCTION_MINIMIZATION_H
#define ONE_D_FUNCTION_MINIMIZATION_H

#include "kernel/base/types.h"
#include "kernel/utilities/common_uitls.h"

#include <cmath>
#include <limits>
#include <utility>

namespace kernel {
namespace maths {
namespace opt {


constexpr real_t GOLD = 1.618034;

///
/// \brief Minimum bracketing algorithm
/// Algorithm adapted from Numerical Recipes in C 1997
///
template<typename FunctionTp>
void min_bracket(real_t ax, real_t bx, real_t cx,
                 const FunctionTp& function){

    // GLIMIT is the maximum magnification
    // allowed for a parabolic-fit step.

    const real_t GLIMIT = 100.0;
    const real_t TINY = 1.0e-20;

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

    real_t ulim = 0.0;
    real_t u = 0.0;
    real_t q = 0.0;
    real_t r = 0.0;
    real_t fu = 0.0;

    while (fb > fc) {

     r=(bx - ax)*(fb - fc);
     q=(bx - cx)*(fb - fa);
     u=bx-((bx - cx)*q - ( bx - ax)*r)/
             (2.0*utils::sign(std::max(std::fabs(q-r),TINY),q-r));
     ulim=bx + GLIMIT*(cx - bx);

     // Test various possibilities:
     if ((bx-u)*(u-cx) > 0.0) {

        //Parabolic u is between b and c: try it.
        fu=function(u);

        if (fu < fc) {
            // Got a minimum between b and c.
             ax= bx;
             bx=u;
             fa=fb;
             fb=fu;
             return;
        }
        else if (fu >  fb) {
          // Got a minimum between between a and u.
          cx=u;
          fc=fu;
         return;
        }

        u= cx+GOLD*(cx - bx);
        //Parabolic fit was no use. Use default magnification.
        fu=function(u);
     }
     else if ((cx - u)*(u-ulim) > 0.0) {
        //Parabolic fit is between c and its allowed limit.
        fu=function(u);

         if (fu < fc) {
            auto tmp1 = cx + GOLD*(cx - bx);
            utils::shift(bx, cx, u, tmp1);

            auto tmp2 = function(u);
            utils::shift(fb, fc,fu, tmp2);
         }
     }
     else if ((u-ulim)*(ulim-cx) >= 0.0) {
      // Limit parabolic u to maximum allowed value.
        u=ulim;
        fu=function(u);
     }
     else {
        // Reject parabolic u, use default magnification.
        u=cx + GOLD*(cx - bx);
        fu=function(u);
     }

     utils::shift(ax, bx, cx , u);
     utils::shift(fa, fb, fc, fu);
    }
}

///
/// \brief Golden section search algorithm for
/// finding 1D function minimum.
/// See: http://fourier.eng.hmc.edu/e176/lectures/ch3/node3.html
/// Given a function f, and given a bracketing triplet of abscissas ax, bx, cx (such that bx is
/// between ax and cx , and f(bx) is less than both f(ax) and f(cx) ), this routine performs a
/// golden section search for the minimum, isolating it to a fractional precision of about tol. The
/// abscissa of the minimum is returned as first , and the minimum function value is returned as
/// second in the returned std::pair
///
/// Implementation adapted from Numerical Recipes in C 1997.
///
template<typename FunctionTp>
std::pair<real_t, real_t>
golden_section_search(real_t ax, real_t bx, real_t cx,
                      const FunctionTp& function, real_t tol){

    constexpr real_t R = 0.61803399;
    constexpr real_t C = (1.0-R);

    real_t x0 = ax;
    real_t x3 = cx;
    real_t x1 = 0.0;
    real_t x2 = 0.0;
    real_t xmin = std::numeric_limits<real_t>::max();

    // Make x0 to x1 the smaller segment,
    //  and fill in the new point to be tried.

    if(std::fabs(cx - bx) > std::fabs(bx - ax)){

        x1 = bx;
        x2 = bx + C*(cx - bx);
    }
    else{
        x2 = bx;
        x1 = bx - C*(bx - ax);
    }

    // The initial function evaluations. Note that
    // we never need to evaluate the function  at the original endpoints.
    auto f1 = function(x1);
    auto f2 = function(x2);

    while (std::fabs(x3-x0) > tol*(std::fabs(x1) + std::fabs(x2))) {
        if (f2 < f1) {
            // One possible outcome, its housekeeping,
            auto tmp = R*x1+C*x3;
            utils::shift(x0, x1, x2, tmp);

            auto tmp2 = function(x2);
            utils::shift(f1,f2,tmp2);
        }
        else {
            //The other outcome,
            auto tmp = R*x2+C*x0;
            utils::shift(x3,x2,x1, tmp);

            auto tmp2 = function(x1);
            utils::shift(f2,f1, tmp2);

        }
    }

    if (f1 < f2) {
     xmin=x1;
     return {xmin, f1};
    }

    xmin=x2;
    return {xmin, f2};
}

}

}

}

#endif // 1D_FUNCTION_MINIMIZATION_H

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


///
/// \brief Minimum bracketing algorithm
/// Algorithm adapted from Numerical Recipes in C 1997
///
template<typename FunctionTp>
void
min_bracket(real_t ax, real_t bx, real_t cx,
                 const FunctionTp& function){

    // GLIMIT is the maximum magnification
    // allowed for a parabolic-fit step.

    const real_t GLIMIT = 100.0;
    const real_t TINY = 1.0e-20;
    const real_t GOLD = 1.618034;

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

///
/// \brief Given a function f, and given a bracketing triplet of abscissas ax, bx, cx
/// (such that bx is between ax and cx, and f(bx) is less than both f(ax) and f(cx) ),
/// this routine isolates the minimum to a fractional precision
/// of about tol using Brent’s method. The abscissa of
/// the minimum is returned as first , and the minimum function value
/// is returned as second in the returned std::pair
///
/// Implementation adapted from Numerical Recipes in C 1997.
///
template<typename FunctionTp>
std::pair<bool, std::pair<real_t, real_t>>
brent(real_t ax, real_t bx, real_t cx, const FunctionTp& function, real_t tol, uint_t max_itrs){


    const real_t CGOLD=0.3819660;
    const real_t ZEPS=1.0e-10;

    real_t d,etemp,fu,p,q,r,tol1,tol2,u,xm;

    real_t xmin = std::numeric_limits<real_t>::max();

    // This will be the distance moved on
    // the step before last.
    real_t e=0.0;

    // a and b must be in ascending order,
    // but input abscissas need not be.
    auto a=(ax < cx ? ax : cx);
    auto b=(ax > cx ? ax : cx);

    auto x = bx;
    auto w = bx;
    auto v = bx;

    auto fw = function(x);
    auto fv = fw;
    auto fx = fw;

    // the main program loop
    for (uint_t iter=1; iter<=max_itrs; ++iter) {

        xm=0.5*(a+b);
        tol1=tol*std::fabs(x)+ZEPS;
        tol2=2.0*tol1;

        // maybe we are done here.
        if (std::fabs(x-xm) <= (tol2-0.5*(b-a))) {
            return {true, {x, fx}};
        }

        // Construct a trial parabolic fit.
        if (std::fabs(e) > tol1) {

            r=(x-w)*(fx-fv);
            q=(x-v)*(fx-fw);
            p=(x-v)*q-(x-w)*r;
            q=2.0*(q-r);
            if (q > 0.0) {
                p = -p;
            }

            q=std::fabs(q);
            etemp=e;
            e=d;

            // The above conditions determine the acceptability of the parabolic fit.
            // Here we take the golden section step into the larger of the two segments.
            if (std::fabs(p) >= std::fabs(0.5*q*etemp) || p <= q*(a-x) || p >= q*(b-x)){
                d=CGOLD*(e=(x >= xm ? a-x : b-x));
            }
            else{

                d=p/q;
                u=x+d;
                if (u-a < tol2 || b-u < tol2){
                    d=utils::sign(tol1,xm-x);
                }
             }
        }
        else{
            d=CGOLD*(e=(x >= xm ? a-x : b-x));
        }

        u=(fabs(d) >= tol1 ? x+d : x+utils::sign(tol1,d));

        //This is the one function evaluation per iteration.
        fu=function(u);

        //Now decide what to do with our function evaluation.
        if (fu <= fx) {

            if (u >= x){
                a=x;
            }
            else{
                b=x;
            }

            utils::shift(v,w,x,u);
            utils::shift(fv,fw,fx,fu);
        }
        else {
            if (u < x){
                a=u;
            }
            else{
                b=u;
            }

            if (fu <= fw || w == x) {
                v=w;
                w=u;
                fv=fw;
                fw=fu;
            }
            else if (fu <= fv || v == x || v == w) {
                v=u;
                fv=fu;
            }
        }
     }// end for

    // we should never get here
    return {false, {x, fx}};
}

}// opt

}// maths

}// kernel

#endif // 1D_FUNCTION_MINIMIZATION_H

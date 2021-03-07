#ifndef LP_METRIC_H
#define LP_METRIC_H

#include "kernel/base/types.h"
#include "kernel/geometry/geom_point.h"

namespace kernel
{

/// \brief The LpMetric class
/// It conforms to the metric policy

template<int P, bool TTakeRoot = true>
class LpMetric
{

public:

   static const int Power = P;
   static const bool TakeRoot = TTakeRoot;
   static real_t tolerance_value;
   typedef real_t cost_t;
   typedef real_t value_t;

   /// \brief Tolerance used by the class
   static real_t tolerance(){return tolerance_value;}

   /// \brief Default constructor. It is required to
   /// satisfy the metric policy
   LpMetric()=default;

   /// \brief Overload operator()
   real_t operator()(const DynVec<real_t>& v1, const DynVec<real_t>& v2)const;

   /// \brief Overload operator
   template<int dim>
   real_t operator()(const GeomPoint<dim>& v1, const GeomPoint<dim>& v2  )const;

   /// \brief evaluate
   static real_t evaluate(const DynVec<real_t>& v1, const DynVec<real_t>& v2);

   /// \brief Evaluate for points
   template<int dim>
   static real_t evaluate(const GeomPoint<dim>& v1, const GeomPoint<dim>& v2);
};

template<int P, bool TTakeRoot>
template<int dim>
real_t
LpMetric<P,TTakeRoot>::operator()(const GeomPoint<dim>& v1, const GeomPoint<dim>& v2  )const{
    return LpMetric<P,TTakeRoot>::evaluate(v1, v2);
}

/// \brief some useful shortcuts
using ManhattanMetric = LpMetric<1, false> ;
using SqrEuclidean_metric =  LpMetric<2, false> ;
using EuclideanMetric = LpMetric<2, true> ;

}

#include "kernel/maths/lp_metric_impl.h"

#endif // LP_METRIC_H

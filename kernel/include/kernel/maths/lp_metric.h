#ifndef LP_METRIC_H
#define LP_METRIC_H

#include "kernel/base/types.h"


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
   typedef real_t cost_t;

   /// \brief Default constructor. It is required to
   /// satisfy the metric policy
   LpMetric()=default;

   /// \brief Overload operator()
   real_t operator()(const DynVec<real_t>& v1, const DynVec<real_t>& v2)const;

   /// \brief evaluate
   static real_t evaluate(const DynVec<real_t>& v1, const DynVec<real_t>& v2);
};

/**
 * \brief some useful shortcuts
 */
using ManhattanMetric = LpMetric<1, false> ;
using SqrEuclidean_metric =  LpMetric<2, false> ;
using EuclideanMetric = LpMetric<2, true> ;

}

#include "kernel/maths/lp_metric_impl.h"

#endif // LP_METRIC_H

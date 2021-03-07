#ifndef LP_METRIC_IMPL_H
#define LP_METRIC_IMPL_H

#include <cmath>

namespace kernel
{

template<int P, bool TTakeRoot>
real_t LpMetric<P,TTakeRoot>::tolerance_value = 1.0e-8;

//the general implementation
template<int P, bool TTakeRoot>
real_t
LpMetric<P,TTakeRoot>::evaluate(const DynVec<real_t>& v1, const DynVec<real_t>& v2){

  real_t sum = 0.0;
  for (uint_t i = 0; i < v1.size(); i++){
      sum += std::pow(std::fabs(v1[i] - v2[i]), P);
  }

  if (!TTakeRoot) // The compiler should optimize this correctly at compile-time.
     return sum;

  return std::pow(sum, (1.0 / P));
}


// L1-metric specializations; the root doesn't matter.
template<>
real_t
LpMetric<1, true>::evaluate(const DynVec<real_t>& v1, const DynVec<real_t>& v2)
{
  return blaze::l1Norm(v1 - v2);
}

template<>
real_t
LpMetric<1, false>::evaluate(const DynVec<real_t>& v1, const DynVec<real_t>& v2)
{
  return blaze::l1Norm(v1 - v2);
}

// L2-metric specializations.
template<>
real_t
LpMetric<2, true>::evaluate(const DynVec<real_t>& v1, const DynVec<real_t>& v2)
{
  return std::sqrt(blaze::sqrNorm(v1 - v2));
}

template<>
real_t
LpMetric<2, false>::evaluate(const DynVec<real_t>& v1, const DynVec<real_t>& v2)
{
  return std::sqrt(blaze::sqrNorm(v1 - v2));
}

// L3-metric specialization (not very likely to be used, but just in case).
template<>
real_t
LpMetric<3, true>::evaluate(const DynVec<real_t>& v1, const DynVec<real_t>& v2)
{
  return std::pow(blaze::reduce(blaze::pow(blaze::abs(v1 - v2), 3.0), blaze::Add()), 1.0 / 3.0);
}

template<>
real_t
LpMetric<3, false>::evaluate(const DynVec<real_t>& v1, const DynVec<real_t>& v2)
{
  return std::pow(blaze::reduce(blaze::pow(blaze::abs(v1 - v2), 3.0), blaze::Add()), 1.0 / 3.0);
}


template<int P, bool TTakeRoot>
real_t
LpMetric<P, TTakeRoot>::operator()(const DynVec<real_t>& v1, const DynVec<real_t>& v2)const{

  return LpMetric<P, TTakeRoot>::evaluate(v1,v2);
}

/// \brief Evaluate for points
template<int P, bool TTakeRoot>
template<int dim>
real_t
LpMetric<P, TTakeRoot>::evaluate(const GeomPoint<dim>& v1, const GeomPoint<dim>& v2){

    real_t sum = 0.0;
    for (uint_t i = 0; i < v1.size(); i++){
        sum += std::pow(std::fabs(v1[i] - v2[i]), P);
    }

    if (!TTakeRoot) // The compiler should optimize this correctly at compile-time.
       return sum;

    return std::pow(sum, (1.0 / P));
}


// L1-metric specializations; the root doesn't matter.
template<>
template<int dim>
real_t
LpMetric<1, true>::evaluate(const GeomPoint<dim>& v1, const GeomPoint<dim>& v2)
{
    DynVec<real_t> vec1(v1.coordinates());
    DynVec<real_t> vec2(v2.coordinates());
    return blaze::l1Norm(vec1 - vec2);
}

template<>
template<int dim>
real_t
LpMetric<1, false>::evaluate(const GeomPoint<dim>& v1, const GeomPoint<dim>& v2)
{
    DynVec<real_t> vec1(v1.coordinates());
    DynVec<real_t> vec2(v2.coordinates());
    return blaze::l1Norm(vec1 - vec2);
}

// L2-metric specializations.
template<>
template<int dim>
real_t
LpMetric<2, true>::evaluate(const GeomPoint<dim>& v1, const GeomPoint<dim>& v2)
{
  return v1.distance(v2);
}

template<>
template<int dim>
real_t
LpMetric<2, false>::evaluate(const GeomPoint<dim>& v1, const GeomPoint<dim>& v2)
{
  return v1.distance(v2);
}

// L3-metric specialization (not very likely to be used, but just in case).
template<>
template<int dim>
real_t
LpMetric<3, true>::evaluate(const GeomPoint<dim>& v1, const GeomPoint<dim>& v2)
{
    DynVec<real_t> vec1(v1.coordinates());
    DynVec<real_t> vec2(v2.coordinates());
    return std::pow(blaze::reduce(blaze::pow(blaze::abs(vec1 - vec2), 3.0), blaze::Add()), 1.0 / 3.0);
}

template<>
template<int dim>
real_t
LpMetric<3, false>::evaluate(const GeomPoint<dim>& v1, const GeomPoint<dim>& v2)
{
    DynVec<real_t> vec1(v1.coordinates());
    DynVec<real_t> vec2(v2.coordinates());
    return std::pow(blaze::reduce(blaze::pow(blaze::abs(vec1 - vec2), 3.0), blaze::Add()), 1.0 / 3.0);
}


}

#endif // LP_METRIC_IMPL_H

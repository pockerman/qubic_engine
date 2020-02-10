#ifndef NUMERIC_VECTOR_FUNCTION_H
#define NUMERIC_VECTOR_FUNCTION_H

#include "kernel/maths/functions/function_base.h"
#include "kernel/geometry/geom_point.h"

namespace kernel {
namespace numerics {

template<int dim>
class NumericVectorFunctionBase: public FunctionBase<DynVec<real_t>, GeomPoint<dim>>
{
public:

    typedef GeomPoint<dim> input_t;
    typedef typename FunctionBase<DynVec<real_t>, GeomPoint<dim>>::output_t output_t;

    /// \brief Destructor
    virtual ~NumericVectorFunctionBase()
    {}

    /// \brief Returns the value of the function
    virtual output_t value(const GeomPoint<dim>&  input)const override{return DynVec<real_t>(0.0);}

    /// \brief Returns the gradients of the function
    virtual DynVec<real_t> gradients(const GeomPoint<dim>&  input)const override{return DynVec<real_t>();}

    /// \brief Returns the number of coefficients
    virtual uint_t n_coeffs()const override{return 0;}

protected:

    NumericVectorFunctionBase();

};

template<int dim>
NumericVectorFunctionBase<dim>::NumericVectorFunctionBase()
    :
      FunctionBase<DynVec<real_t>, GeomPoint<dim>>()
{}

}

}

#endif // NUMERIC_VECTOR_FUNCTION_H

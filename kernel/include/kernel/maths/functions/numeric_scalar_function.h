#ifndef NUMERIC_SCALAR_FUNCTION_H
#define NUMERIC_SCALAR_FUNCTION_H

#include "kernel/maths/functions/function_base.h"
#include "kernel/geometry/geom_point.h"
namespace kernel{
namespace numerics{


/// \brief The general template
template<int dim>
class NumericScalarFunction: public FunctionBase<real_t, GeomPoint<dim>>
{
public:

    typedef GeomPoint<dim> input_t;
    typedef typename FunctionBase<real_t, GeomPoint<dim>>::output_t output_t;

    /// \brief Destructor
    virtual ~NumericScalarFunction()
    {}

    /// \brief Returns the value of the function
    virtual output_t operator()(const GeomPoint<dim>& input)const{return value(input);}

    /// \brief Returns the value of the function
    virtual output_t value(const GeomPoint<dim>&  input)const override{return 0.0;}

    /// \brief Returns the gradients of the function
    virtual DynVec<real_t> gradients(const GeomPoint<dim>&  input)const override{return DynVec<real_t>();}

    /// \brief Returns the gradient magnitude
    virtual real_t gradient_mag(const GeomPoint<dim>&  input)const{return blaze::l2Norm(gradients(input));}

    /// \brief Returns the number of coefficients
    virtual uint_t n_coeffs()const override{return 0;}

protected:

    NumericScalarFunction();

};

template<int dim>
NumericScalarFunction<dim>::NumericScalarFunction()
    :
      FunctionBase<real_t, GeomPoint<dim>>()
{}


}
}

#endif // NUMERIC_SCALAR_FUNCTION_H

#ifndef DUMMY_FUNCTION_H
#define DUMMY_FUNCTION_H

#include "kernel/base/types.h"
#include "kernel/maths/functions/function_base.h"

namespace kernel
{

template<typename OutputType, typename... InputType>
class DummyFunction: public FunctionBase<OutputType, InputType...>
{
public:

    typedef typename FunctionBase<OutputType, InputType...>::output_t output_t;

    /**
     * Returns the value of the function
     */
    virtual output_t value(const InputType&...  /*input*/)const override final {return output_t();}

    template<typename Executor, typename Options>
    output_t value(Executor&, const Options&, const InputType&... )const{return output_t();}

    /**
      * Returns the gradients of the function
      */
    virtual DynVec<real_t> gradients(const InputType&...  /*input*/)const override final {return DynVec<real_t>();}

    /**
      * Returns the number of coefficients
      */
    virtual uint_t n_coeffs()const override final {return 0;}

};

}

#endif // DUMMY_FUNCTION_H

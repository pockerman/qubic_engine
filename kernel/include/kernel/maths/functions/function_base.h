#ifndef FUNCTION_BASE_H
#define FUNCTION_BASE_H


#include "kernel/base/types.h"

namespace kernel
{

template<typename InputTp, typename OutputTp>
class FunctionBase
{


public:

    typedef InputTp input_t;
    typedef OutputTp output_t;

    /**
      * Destructor
      */
    virtual ~FunctionBase()
    {}

    /**
     * Returns the value of the function
     */
    virtual output_t value(const input_t& input)=0;

    /**
      * Returns the number of coefficients
      */
    virtual uint_t n_coeffs()const=0;

protected:

    /**
     * Constructor
     */
    FunctionBase();

};


template<typename InputTp, typename OutputTp>
inline
FunctionBase<InputTp, OutputTp>::FunctionBase()
{}

}

#endif // FUNCTION_BASE_H

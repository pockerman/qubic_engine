#ifndef FORCE_CALCULATOR_BASE_H
#define FORCE_CALCULATOR_BASE_H

#include "kernel/base/types.h"

#include "boost/any.hpp"

#include <map>
#include <string>

namespace kernel {
namespace models {

class ForceCalculatorBase
{
public:

    ///
    /// \brief force_t The type of the force
    ///
    typedef DynVec<real_t> force_t;

    ///
    /// \brief input_t The type of the input the calculatro is using
    ///
    typedef std::map<std::string, boost::any> input_t;

    ///
    /// \brief ~ForceCalculatorBase Desrtuctor
    ///
    virtual ~ForceCalculatorBase(){}

    ///
    /// \brief compute_forces Compute the forces
    /// acting on the chassis
    ///
    virtual force_t compute_force(const input_t& input)const=0;

    ///
    /// \brief input_dim The dimension of the input
    ///
    uint_t input_dim()const{return input_dim_;}

    ///
    /// \brief output_dim The dimension of the output
    ///
    uint_t output_dim()const{return output_dim_;}

protected:

    ///
    /// \brief ForceCalculatorBase Constructor
    ///
    ForceCalculatorBase(uint_t output_dim, uint_t input_dim);

private:

    ///
    /// \brief output_dim_ The dimension of the output
    ///
    uint_t output_dim_;

    ///
    /// \brief input_dim_ The dimension of the inputput
    ///
    uint_t input_dim_;

};

inline
ForceCalculatorBase::ForceCalculatorBase(uint_t output_dim, uint_t input_dim)
    :
    output_dim_(output_dim),
    input_dim_(input_dim)
{}

}

}

#endif // FORCE_CALCULATOR_BASE_H

#ifndef LINEAR_PARAMETRIC_MODEL_H
#define LINEAR_PARAMETRIC_MODEL_H

#include "kernel/base/config.h"
#include "cubic_engine/base/cubic_engine_types.h"
#include "kernel/maths/functions/real_vector_polynomial.h"

#include <boost/noncopyable.hpp>

#ifdef KERNEL_DEBUG
#include <cassert>
#endif

#include <vector>
#include <ostream>


namespace cengine{
namespace ml {

///
///
///
template<typename ValueTp>
class LinearParametricModel: private boost::noncopyable
{
public:

    ///
    /// \brief value_t The result value type
    ///
    typedef ValueTp value_t;

    ///
    /// \brief LinearParametricModel
    ///
    LinearParametricModel(uint_t n_features, bool use_itercept);

    ///
    /// \brief get_parameters. Returns the parameters of the model
    ///
    DynVec<real_t> get_parameters()const{return polynomial_.coeffs();}

    ///
    /// \brief get_interception
    ///
    real_t get_interception()const;

    ///
    /// \brief update_parameters
    ///
    void update_parameters(const std::vector<real_t>& parameters){polynomial_.set_coeffs(parameters);}

    ///
    /// \brief Print the model coeffs
    ///
    std::ostream& print(std::ostream& out)const;


protected:

    ///
    /// \brief num_features_
    ///
    uint_t num_features_;

    ///
    /// \brief use_intercept_
    ///
    bool use_intercept_;

    ///
    /// \brief polynomial_ The polynomial
    ///
    kernel::PolynomialFunction polynomial_;

};

template<typename ValueTp>
LinearParametricModel<ValueTp>::LinearParametricModel(uint_t n_features, bool use_intercept)
    :
      num_features_(n_features),
      use_intercept_(use_intercept),
      polynomial_()
 {

     num_features_ = use_intercept ? n_features + 1 : n_features;
     std::vector<int> order(num_features_, 1);
     order[0] = 0;
     polynomial_.create_from(DynVec<real_t>(num_features_, 0.0), order);
 }

template<typename ValueTp>
std::ostream&
LinearParametricModel<ValueTp>::print(std::ostream& out)const{

    for(uint_t c=0; c < polynomial_.n_coeffs(); ++c){
            out<<polynomial_.coeff(c);

            if(c == polynomial_.n_coeffs() - 1){
                out<<"\n";
            }
            else{
                out<<",";
            }
        }

        return out;
}

template<typename ValueTp>
real_t
LinearParametricModel<ValueTp>::get_interception()const{

#ifdef KERNEL_DEBUG
     assert(use_intercept_ && "No intercept term defined");
#endif

     return polynomial_.coeff(0);
}


inline
std::ostream& operator<<(std::ostream& out,
                         const LinearParametricModel<real_t>& classifier){
    return classifier.print(out);
}

inline
std::ostream& operator<<(std::ostream& out,
                         const LinearParametricModel<uint_t>& classifier){
    return classifier.print(out);
}

}
}

#endif // LINEAR_PARAMETRIC_MODEL_H

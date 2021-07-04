#ifndef VECTOR_POLYNOMIAL_H
#define VECTOR_POLYNOMIAL_H

#include "kernel/base/types.h"
#include "kernel/maths/functions/monomial.h"
#include <vector>

namespace kernel {

///
/// \brief The VectorPolynomial class. Models a vector
/// polynomial of the form y = a_1*x_1^k_1 + a_2*x_2^k_2 + ....
///
class VectorPolynomial
{
public:

    ///
    /// \brief default constructor
    ///
    VectorPolynomial() = default;

    ///
    /// \brief VectorPolynomial. Create a vector polynomial where
    /// all terms have order one
    ///
    VectorPolynomial(const std::vector<real_t>& coeffs);

    ///
    /// \brief VectorPolynomial. Create a vector polynomial  from
    /// the given monomials
    ///
    VectorPolynomial(const std::vector<Monomial>& mono);

    ///
    /// \brief Returns the value of the polynomial at the given point
    ///
    template<typename PointTp>
    real_t value(const PointTp& point)const;

    ///
    /// \brief n_coeffs
    /// \return
    ///
    uint_t n_coeffs()const{return monomials_.size();}

    ///
    /// \brief get_parameters
    /// \return
    ///
    std::vector<real_t> get_parameters()const;

    ///
    /// \brief set_coeffs
    /// \param parameters
    ///
    void set_coeffs(const std::vector<real_t>& parameters);

private:


    ///
    /// \brief monomials_
    ///
    std::vector<Monomial> monomials_;


};

}

#endif // VECTOR_POLYNOMIAL_H

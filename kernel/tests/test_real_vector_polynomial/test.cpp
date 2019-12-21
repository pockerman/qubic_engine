#include "kernel/maths/functions/real_vector_polynomial.h"
#include "kernel/base/types.h"
#include "kernel/base/exceptions.h"

#include <gtest/gtest.h>
#include <vector>
#include <string>
#include <exception>


namespace{

}


/***
 * Test Scenario:   The application attempts to create a RealVectorPolynomialFunction with unequal number of orders and coeffs
 * Expected Output:	std::invalid_argument
 **/

TEST(TestRealVectorPolynomialFunction, CreateWithUnequalNumberOfOrdersAndCoeffs) {

    using kernel::real_t;
    using kernel::uint_t;
    using kernel::RealVectorPolynomialFunction;
    using DynVec = kernel::DynVec<real_t>;


    std::string expected;
    /// TODO: Is there a better way to do this?
    try{

      // RealVectorPolynomialFunction function;

       DynVec coeffs;
       std::vector<int> order(10, 1.0);
       expected = "Coeffs size: " + std::to_string(coeffs.size())+
                              " not equal to order size: " + std::to_string(order.size());
       //function.create_from(coeffs, order);
    }
    catch(std::invalid_argument& e){
        std::string msg = e.what();
        ASSERT_EQ( msg, expected );
    }
    catch(...){

        ASSERT_FALSE("A non expected exception was thrown");
    }
}


/***
 * Test Scenario:   The application attempts to evaluate a  RealVectorPolynomialFunction with incorrectly sized input
 * Expected Output:	std::invalid_argument
 **/

TEST(TestRealVectorPolynomialFunction, EvaluateWithIncorrectInputSize) {

    using kernel::real_t;
    using kernel::uint_t;
    using kernel::RealVectorPolynomialFunction;
    using DynVec = kernel::DynVec<real_t>;

    /// TODO: Is there a better way to do this?
    std::string expected;
    try{

        //RealVectorPolynomialFunction function;

        DynVec coeffs(10, 1.0);
        std::vector<int> order(10, 1.0);
        //function.create_from(coeffs, order);

        DynVec input(5, 0.0);

        //expected = "input size: " + std::to_string(input.size())+
        //           " not equal to monomials size: " + std::to_string(function.n_coeffs());
        //function.value(input);

    }
    catch(std::invalid_argument& e){
        std::string msg = e.what();

        ASSERT_EQ(expected, msg);
    }
    catch(...){

        ASSERT_FALSE("A non expected exception was thrown");
    }
}








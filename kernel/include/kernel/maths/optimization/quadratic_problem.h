#ifndef QUADRATIC_PROBLEM_H
#define QUADRATIC_PROBLEM_H

namespace kernel {
namespace maths {
namespace opt {

template<typename MatrixTp, typename VectorTp>
struct QuadraticProblem
{

    typedef MatrixTp matrix_t;
    typedef VectorTp vector_t;

    matrix_t P;
    matrix_t A;
    vector_t x;
    vector_t q;
    vector_t z;

    ///
    /// \brief l. Min constraints vector
    ///
    vector_t l;

    ///
    /// \brief u. Max constraints vector
    ///
    vector_t u;

};



}

}

}

#endif // QUADRATIC_PROBLEM_H

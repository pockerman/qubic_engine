#ifndef FV_VECTOR_MATRIX_H
#define FV_VECTOR_MATRIX_H

#include <map>
#include <string>

namespace kernel{
namespace numerics{


/// \brief Wrapper to
template<typename MatrixPolicy>
class FVVectorMatrix
{
public:

    typedef MatrixPolicy matrix_policy_t;
    typedef typename matrix_policy_t::matrix_t matrix_t;
    typedef typename matrix_policy_t::vector_t vector_t;
    typedef typename matrix_policy_t::solver_t solver_t;
    typedef typename matrix_policy_t::solver_output_t solver_output_t;

private:

    std::map<std::string, matrix_t> matrices_;

    /// \brief Flag indicating that relaxation
    /// is used. If this is true a different assemply
    /// policy is used...
    bool use_relax_;

};

}
}

#endif // FV_VECTOR_MATRIX_H

#ifndef TD_H
#define TD_H

#include "cubic_engine/base/config.h"

#ifdef USE_RL

#include "cubic_engine/base/cubic_engine_types.h"
#include <functional>

namespace cengine {
namespace rl{

/// \brief Forward declarations
template<typename...> class TD;

///\brief Class for learning state-value functions
/// using TD

template<typename StateTp>
class TD<StateTp>
{
public:

    typedef StateTp state_t;

    using value_function_t  = std::function<real_t (const DynVec<real_t>&, const state_t&)>;
    using grad_value_function_t = std::function<const DynVec<real_t> (const DynVec<real_t>&, const state_t&)>;

    /// \brief learn for a non-terminal state
    /// transitioning from s-->sprime with reward r
    void train(const state_t& s, const state_t& sprime, real_t r);

    /// \brief learn for a terminal state
    void train(const state_t& state, real_t r);

    /// \brief compute error
    real_t td_error(const state_t& s, const state_t& sprime, real_t r) {
       return r + gamma_*v_(theta_, sprime) - v_(theta_,s);
    }


private:

    /// \brief The discount factor
    real_t gamma_;

    /// \brief The learning rate
    real_t alpha_;

    /// \brief The parameter vector for the V-function
    DynVec<real_t> theta_;

    /// \brief A temporary vector holding the gradient of the value function
    DynVec<real_t> grad_;

    value_function_t v_;
    grad_value_function_t gv_;


};

}
}

#endif
#endif // TD_H

#ifndef GD_CONTROL_H
#define GD_CONTROL_H

#include "kernel/base/types.h"
#include "kernel/utilities/iterative_algorithm_controller.h"
#include "kernel/base/kernel_consts.h"

#include <string>
#include <map>
#include <any>

namespace kernel{
namespace numerics {
namespace opt {

///
/// \brief The GDConfig struct Configuration for
/// Gradient descent
///
struct GDConfig: public kernel::IterativeAlgorithmController
{
    ///
    /// \brief DEFAULT_LEARNING_RATE
    ///
    constexpr static real_t DEFAULT_LEARNING_RATE = 0.01;

    ///
    /// \brief The learning rate
    ///
    real_t learning_rate;

    ///
    /// \brief Constructor
    ///
    GDConfig( uint_t max_num_itrs,
               real_t tolerance=kernel::KernelConsts::tolerance(),
               real_t eta=GDConfig::DEFAULT_LEARNING_RATE );

    ///
    /// \brief Constructor
    ///
    GDConfig(const std::map<std::string, std::any>& options);

    ///
    /// \brief reset
    ///
    virtual void reset(const GDConfig& control)final;

};

inline
GDConfig::GDConfig( uint_t max_num_itrs, real_t tolerance, real_t eta_ )
    :
kernel::IterativeAlgorithmController(max_num_itrs,  tolerance),
learning_rate(eta_)
{}



}
}
}

#endif // GD_CONTROL_H

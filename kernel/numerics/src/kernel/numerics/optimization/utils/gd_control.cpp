#include "kernel/numerics/optimization/utils/gd_control.h"

namespace kernel{
namespace numerics{
namespace opt{

GDConfig::GDConfig(const std::map<std::string, std::any>& options)
    :
    GDConfig(100, kernel::KernelConsts::tolerance(), GDConfig::DEFAULT_LEARNING_RATE)
{
    auto itr = options.find("max_num_itrs");

    if(itr != options.end()){
        set_max_itrs(std::any_cast<uint_t>(itr->second));
    }

    itr = options.find("tolerance");
    if(itr != options.end()){
        set_tolerance(std::any_cast<real_t>(itr->second));
    }

    itr = options.find("learning_rate");
    if(itr != options.end()){
        learning_rate = std::any_cast<real_t>(itr->second);
    }
}
}
}
}

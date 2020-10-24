#include "kernel/models/chassis_helper.h"
#include "kernel/base/kernel_consts.h"
#include "kernel/base/physics_constants.h"

namespace kernel{
namespace models {

ChassisHelper::ChassisHelper()
    :
      file_description_(KernelConsts::dummy_string()),
      metric_system_(PhysicsConsts::default_metric_system()),
      scalar_props_()
{}

ChassisHelper::~ChassisHelper()
{}

real_t
ChassisHelper::get_scalar_property(const std::string& name)const{

    auto itr = scalar_props_.find(name);

    if(itr == scalar_props_.end()){
        throw std::logic_error("Scalar property does not exist: " + name);
    }

    return itr->second;
}

void
ChassisHelper::set_scalar_property(const std::string& name, real_t value){
    scalar_props_.insert_or_assign(name, value);
}

}
}

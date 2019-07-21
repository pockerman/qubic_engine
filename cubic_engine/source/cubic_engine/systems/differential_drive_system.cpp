#include "cubic_engine/systems/differential_drive_system.h"
#include "parframe/utilities/map_utilities.h"
namespace cengine
{

void
DiffDriveSystem::set_scalar_property(const std::string& name, real_t value){
    kernel::add_or_update_map(scalar_properties_, name, value);
}

real_t
DiffDriveSystem::get_scalar_property(const std::string& name)const{

    auto itr = scalar_properties_.find(name);

    if(itr == scalar_properties_.end()){
        throw std::invalid_argument( name +" is not a valid name property ");
    }

    return itr->second;
}

}

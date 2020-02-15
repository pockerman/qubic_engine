#ifndef SCALAR_VARIABLE_H
#define SCALAR_VARIABLE_H

#include "kernel/base/types.h"
#include <string>

namespace kernel {

namespace numerics {

class ScalarVar
{
public:

    /// \brief The name of the variable
    ScalarVar(std::string name);

    /// \brief how many components the variable has
    uint_t n_components()const{return 1;}

    /// \brief Returns the name of the variable
    std::string_view name()const{return name_;}

private:

    std::string name_;

};



}

}

#endif // SCALAR_VARIABLE_H

#ifndef DOF_OBJECT_H
#define DOF_OBJECT_H

#include "kernel/base/types.h"
#include "kernel/discretization/dof.h"

#include <vector>
#include <string>
#include <map>

namespace kernel{
namespace numerics {

/// \brief A DofObject holds DoF objects
class DoFObject
{
public:

    /// \brief Constructor
    DoFObject();

    /// \brief How many dofs the object has
    uint_t n_dofs()const{return dofs_.size();}

    /// \brief Insert a new DoF. Throws and exception
    /// if the DoF already exists
    void insert_dof(DoF&& dof);

    /// \brief Returns true if the variable with the
    /// given name exists
    bool has_variable(std::string_view variable)const;

    /// \brief Invalidate the dof indeces for the variable
    /// with the given name
    void invalidate_dofs(std::string_view name);

    /// \brief Returns the DoF for the given variable
    DoF get_dof(std::string_view name)const;

private:

    /// \brief Dofs associated with the variable
    std::map<std::string_view, DoF> dofs_;
};
}

}

#endif // DOF_OBJECT_H

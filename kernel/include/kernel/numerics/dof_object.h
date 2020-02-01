#ifndef DOF_OBJECT_H
#define DOF_OBJECT_H

#include "kernel/base/types.h"
#include <vector>
#include <string>

namespace kernel
{

struct DoF
{
    std::string var_name;
    uint_t id;
    bool active;

    static void invalidate_dof(DoF& dof);
    static void invalidate_dof(DoF&& dof);
};

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
    bool has_variable(const std::string& variable)const;

private:

    /// \brief List dof managed by the object
    std::vector<DoF> dofs_;
};

}

#endif // DOF_OBJECT_H

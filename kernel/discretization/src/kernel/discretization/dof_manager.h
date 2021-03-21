#ifndef DOF_MANAGER_H
#define DOF_MANAGER_H

#include "kernel/base/types.h"

#include <string>

namespace kernel{
namespace numerics {

template<int dim> class Mesh;
template<int dim> class Element;
class ScalarVar;
class DoF;

template<int dim>
class FVDoFManager
{
public:

    /// \brief Constructor
    FVDoFManager();

    /// \brief Destructor
    virtual ~FVDoFManager();

    /// \brief Distibute the dofs
    template<typename VarTp>
    void distribute_dofs(Mesh<dim>& mesh, const VarTp& var);

    /// \brief Get the dofs on the given elem
    void get_dofs(const Element<dim>& elem, std::vector<DoF>& dofs)const;

    /// returns the number of dofs
    uint_t n_dofs()const{return n_dofs_;}

    /// \brief Returns the variable the dof manager
    /// is working on
    std::string_view var_name()const{return var_name_;}

protected:

    /// \brief Invalidate the dofs
    void invalidate_dofs(Mesh<dim>& mesh);

    /// \brief Invalidate the dofs
    void do_distribute_dofs(Mesh<dim>& mesh);

private:

    /// \brief The number of DoFs the manager distributed
    uint_t n_dofs_;

    /// \brief The name of the variable the manager is working on
    std::string_view var_name_;
};

template<int dim>
template<typename VarTp>
void
FVDoFManager<dim>::distribute_dofs(Mesh<dim>& mesh, const VarTp& var){

    n_dofs_ = 0;
    var_name_ = var.name();

    invalidate_dofs(mesh);
    do_distribute_dofs(mesh);
}

}
}

#endif // DOF_MANAGER_H

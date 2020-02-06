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
    void distribute_dofs(Mesh<dim>& mesh, const ScalarVar& var);

    /// \brief Get the dofs on the given elem
    void get_dofs(const Element<dim>& elem, std::vector<DoF>& dofs)const;

    /// returns the number of dofs
    uint_t n_dofs()const{return n_dofs_;}

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

}
}

#endif // DOF_MANAGER_H

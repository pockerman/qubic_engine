#ifndef MESH_ENTITY_H
#define MESH_ENTITY_H

#include "kernel/base/types.h"
#include "kernel/base/kernel_consts.h"

namespace kernel
{

namespace numerics
{

/// \brief Helper class that wraps non template dependent
/// parameters and common to every mesh entity
class MeshEntity
{

public:

    /// \brief Constructor
    MeshEntity();

    /// \brief Constructor
    MeshEntity(uint_t id, uint_t pid);

    /// \brief Returns the id of the element
    uint_t get_id()const{return id_;}

    /// \brief Set the id of the element
    void set_id(uint_t id){id_ = id;}

    /// \brief Returns the id of the element
    uint_t get_pid()const{return pid_;}

    /// \brief Set the id of the element
    void set_pid(uint_t id){pid_ = id;}

    bool has_valid_id()const{return true;}

    bool is_active()const{return is_active_;}

    void set_active_flag(bool f){is_active_ = f;}

private:

    /// \brief The id of the element
    uint_t id_;

    /// \brief The processor id the element belongs
    uint_t pid_;

    /// \brief flag indicating if the entity is active
    bool is_active_;

};

inline
MeshEntity::MeshEntity()
    :
    id_(KernelConsts::invalid_size_type()),
    pid_(KernelConsts::invalid_size_type()),
    is_active_(true)
{}


inline
MeshEntity::MeshEntity(uint_t id, uint_t pid)
    :
   id_(id),
   pid_(pid),
   is_active_(true)
{}

}

}

#endif // MESH_ENTITY_H

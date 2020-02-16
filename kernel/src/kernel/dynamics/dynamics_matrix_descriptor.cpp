#include "kernel/dynamics/dynamics_matrix_descriptor.h"

#include <stdexcept>
namespace kernel{
namespace dynamics{

DynamicsMatrixDescriptor::DynamicsMatrixDescriptor()
    :
    matrices_(),
    vectors_()
{}

DynamicsMatrixDescriptor::matrix_iterator
DynamicsMatrixDescriptor::find_matrix(const std::string& name){
    return matrices_.find(name);
}

DynamicsMatrixDescriptor::matrix_t&
DynamicsMatrixDescriptor::get_matrix(const std::string& name){

    auto itr = matrices_.find(name);

    if(itr != matrices_.end()){
        return itr->second;
    }

    throw std::logic_error("Matrix " + name + " not found");
}
const DynamicsMatrixDescriptor::matrix_t&
DynamicsMatrixDescriptor::get_matrix(const std::string& name)const{

    auto itr = matrices_.find(name);

    if(itr != matrices_.end()){
        return itr->second;
    }

    throw std::logic_error("Matrix " + name + " not found");
}

DynamicsMatrixDescriptor::vector_t&
DynamicsMatrixDescriptor::get_vector(const std::string& name){

    auto itr = vectors_.find(name);

    if(itr != vectors_.end()){
        return itr->second;
    }

    throw std::logic_error("Vector " + name + " not found");
}
const DynamicsMatrixDescriptor::vector_t&
DynamicsMatrixDescriptor::get_vector(const std::string& name)const{
    auto itr = vectors_.find(name);

    if(itr != vectors_.end()){
        return itr->second;
    }

    throw std::logic_error("Vector " + name + " not found");
}

}
}

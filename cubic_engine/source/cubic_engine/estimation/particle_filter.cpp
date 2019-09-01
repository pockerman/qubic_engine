#include "cubic_engine/estimation/particle_filter.h"

namespace cengine
{


ParticleFilterMatrixDescriptor::ParticleFilterMatrixDescriptor()
:
matrix_maps_()
{}


const std::vector<std::string>
ParticleFilterMatrixDescriptor::get_names()const{

    return {"A", "B", "H", "P", "Q", "R"};
}


void
ParticleFilterMatrixDescriptor::set_matrix(const std::string& name, matrix_type& matrix){

    auto names = get_names();
    auto itr = std::find(names.cbegin(), names.cend(), name);

    if(itr == names.end()){

        throw  std::invalid_argument("Matrix name "+name+" not in [A, B, H, P, Q, R]");
    }

    matrix_maps_[name] = &matrix;
}


const ParticleFilterMatrixDescriptor::matrix_type*
ParticleFilterMatrixDescriptor::get_matrix(const std::string& name)const{

    auto itr = matrix_maps_.find(name);

    if(itr == matrix_maps_.end()){

        throw  std::invalid_argument("Matrix name "+name+" not in [A, B, H, P, Q, R]");
    }

    return itr->second;
}


ParticleFilterMatrixDescriptor::matrix_type*
ParticleFilterMatrixDescriptor::get_matrix(const std::string& name){

    return const_cast<ParticleFilterMatrixDescriptor::matrix_type*>(static_cast<const ParticleFilterMatrixDescriptor&>(*this).get_matrix(name));
}

}

#ifndef DYNAMICS_MATRIX_DESCRIPTOR_H
#define DYNAMICS_MATRIX_DESCRIPTOR_H

#include "kernel/base/types.h"

#include <map>
#include <string>

namespace kernel{
namespace dynamics {


///
/// \brief The DynamicsMatrixDescriptor class. Helper class
/// to model the matrix representon of dynamical systems
///
class DynamicsMatrixDescriptor{

public:

    typedef DynMat<real_t> matrix_t;
    typedef DynVec<real_t> vector_t;

    typedef std::map<std::string, matrix_t>::iterator matrix_iterator;
    typedef std::map<std::string, matrix_t>::const_iterator const_matrix_iterator;

    typedef std::map<std::string, vector_t>::iterator vector_iterator;
    typedef std::map<std::string, vector_t>::const_iterator const_vector_iterator;

    ///
    /// \brief Constructor
    ///
    DynamicsMatrixDescriptor();

    ///
    /// \brief Destructor
    ///
    ~DynamicsMatrixDescriptor()
    {}

    ///
    /// \brief find the matrix with the given name
    ///
    matrix_iterator find_matrix(const std::string& name);

    ///
    /// \brief find the matrix with the given name
    ///
    const_matrix_iterator find_matrix(const std::string& name)const;

    ///
    /// \brief find the matrix with the given name
    ///
    vector_iterator find_vector(const std::string& name);

    ///
    /// \brief find the matrix with the given name
    ///
    const_vector_iterator find_vector(const std::string& name)const;

    matrix_t& get_matrix(const std::string& name);
    const matrix_t& get_matrix(const std::string& name)const;
    void set_matrix(const std::string& name, const matrix_t& mat){matrices_.insert_or_assign(name, mat);}
    bool has_matrix(const std::string& name)const;


    vector_t& get_vector(const std::string& name);
    const vector_t& get_vector(const std::string& name)const;
    void set_vector(const std::string& name, const vector_t& vec){vectors_.insert_or_assign(name, vec);}

    ///
    /// \brief iterators
    ///
    matrix_iterator matrix_begin(){return matrices_.begin();}
    matrix_iterator matrix_end(){return matrices_.end();}
    const_matrix_iterator matrix_begin()const{return matrices_.cbegin();}
    const_matrix_iterator matrix_end()const{return matrices_.cend();}

    ///
    /// \brief iterators
    ///
    vector_iterator vector_begin(){return vectors_.begin();}
    vector_iterator vector_end(){return vectors_.end();}
    const_vector_iterator vector_begin()const{return vectors_.cbegin();}
    const_vector_iterator vector_end()const{return vectors_.cend();}

protected:

    std::map<std::string, matrix_t> matrices_;
    std::map<std::string, vector_t> vectors_;
};

}
}

#endif // DYNAMICS_MATRIX_DESCRIPTOR_H

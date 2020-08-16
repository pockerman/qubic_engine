#ifndef TRILINOS_EPETRA_VECTOR_H
#define TRILINOS_EPETRA_VECTOR_H

#include "kernel/base/config.h"

#ifdef USE_TRILINOS

#include "kernel/base/types.h"

#include <Epetra_Vector.h>
#include <Epetra_SerialComm.h>
#include <Epetra_Map.h>

#include <ostream>

namespace kernel{
namespace maths {
namespace solvers{

/// forward declaration to Amesos2 wrapper class
class AmesosDirect;
}
}
namespace numerics{

/// forward declarations
class TrilinosKrylovSolver;

///
/// \brief The TrilinosEpetraVector class This is a wrapper to the
/// serial Trilinos Epetra_Vector class
///
class TrilinosEpetraVector{

public:

    ///
    /// \brief The data type the vector holds
    ///
    typedef real_t value_t;

    ///
    /// \brief iterator Pointer type to allow iterator like functionality
    ///
    typedef value_t* iterator;

    ///
    /// \brief trilinos_vector_t The type of the Trilinos vector the class wraps
    ///
    typedef Epetra_Vector trilinos_vector_t;

    ///
    /// \brief Constructor
    ///
    TrilinosEpetraVector();

    ///
    /// \brief How many elements the vector has
    ///
    uint_t size()const;

    ///
    /// \brief How many elements the vector has
    ///
    uint_t global_size()const{return size();}

    ///
    /// \brief Zero the entries
    ///
    void zero();

    ///
    /// \brief Initialize the vector with the given size \p n. Local and global
    /// sizes will be the same
    /// if \p fast is set to false we set the entries of the
    /// vector to zero. The default is to initialize to zero
    ///
    void init(uint_t n, bool fast=false);

    ///
    /// \brief Initialize the vector with n entries and set all entries to val
    ///
    void init(uint_t n, real_t val);

    ///
    /// \brief Add val to the i-th entry of the vector
    ///
    void add(uint_t i,real_t val);

    ///
    /// \brief Set the i-th entry of the vector  to val
    ///
    void set_entry(uint_t i, real_t val);

    ///
    /// \brief Scale the vector with the given factor
    ///
    void scale(real_t factor);

    ///
    /// \brief Compress the vector
    ///
    void compress();

    ///
    /// \brief For the given global indices add the the given values
    ///
    void add(const std::vector<uint_t>& dofs, const std::vector<real_t>& vals);

    ///
    /// \brief Read access to the i-th entry of the vector
    ///
    real_t operator[](uint_t i)const;

    ///
    /// \brief read/write access to the i-th entry of the vector
    ///
    real_t& operator[](uint_t i);

    ///
    /// \brief Returns the i-th entry of the vector
    ///
    real_t get(uint_t i)const{return (*this)[i];}

    ///
    /// \brief Returns the start of the vector
    ///
    iterator begin();

    ///
    /// \brief Returns one past the end of the vector
    ///
    iterator end();

    ///
    /// \brief Print the vector
    ///
    std::ostream& print(std::ostream& out)const;

    ///
    /// \brief get_vector Returns the raw pointer to the Epetra_Vector
    ///
    Epetra_Vector* get_vector(){return vec_.get();}

    ///
    /// \brief get_vector Returns the raw pointer to the Epetra_Vector
    ///
    const Epetra_Vector* get_vector()const{return vec_.get();}

private:

    ///
    /// \brief Pointer to the actual Epetra_Vector
    ///
    std::unique_ptr<Epetra_Vector> vec_;

    ///
    /// \brief Pointer to the map needed by the
    /// Epetra_Vector
    ///
    std::unique_ptr<Epetra_Map> epetra_map_;

    Epetra_CombineMode last_action_;

    ///
    /// \brief The communicator we use.
    /// The Trilinos::Epetra_SerialComm has default ctor
    ///
    Epetra_SerialComm comm_;

    ///
    /// \brief Allow acces to TrilinosKrylovSolver class
    ///
    //friend class TrilinosKrylovSolver;

    ///
    /// \brief Allow access to Amesos2Direct class
    ///
    //friend class Amesos2Direct;
};


}

}
#endif
#endif // TRILINOS_EPETRA_VECTOR_H

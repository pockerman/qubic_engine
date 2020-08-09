#ifndef TRILINOS_EPETRA_MULTIVECTOR_H
#define TRILINOS_EPETRA_MULTIVECTOR_H

#include "kernel/base/config.h"

#ifdef USE_TRILINOS

#include "kernel/base/types.h"

// Trilinos
#include <Epetra_MultiVector.h>
#include <Epetra_SerialComm.h>
#include <Epetra_Map.h>

namespace kernel {
namespace algebra{


///
/// \brief The TrilinosEpetraMultiVector class This is a wrapper to the
/// serial Trilinos Epetra_MultiVector class.
/// A multi-vector is a collection of one or more vectors,
/// all having the same length and distribution.
///
class TrilinosEpetraMultiVector{

public:

    ///
    /// \brief The data type the vector holds
    ///
    typedef real_t value_t;

    ///
    /// \brief iterator Pointer type to allow iterator like functionality
    ///
    //typedef value_t* iterator;

    ///
    /// \brief trilinos_vector_t The type of the Trilinos vector the class wraps
    ///
    typedef Epetra_MultiVector trilinos_vector_t;

    ///
    /// \brief Constructor
    ///
    TrilinosEpetraMultiVector();

    ///
    /// \brief Constructor. Construct a multi-vector
    /// with n vectors each of which has m entries
    ///
    TrilinosEpetraMultiVector(uint_t n, uint_t m, real_t val);

    ///
    /// \brief How many elements the vector has
    ///
    uint_t size()const;

    ///
    /// \brief How many elements the vector has
    ///
    uint_t global_size()const{return size();}

    ///
    /// \brief n_vectors Returns the number of vectors in the multi-vector
    ///
    uint_t n_vectors()const;

    ///
    /// \brief Returns the i-th entry of the n-th vector
    ///
    real_t get(uint n, uint_t i)const;

    ///
    /// \brief Print the vector
    ///
    std::ostream& print(std::ostream& out)const;

    ///
    /// \brief get_vector Returns the raw pointer to the Epetra_MultiVector
    ///
    trilinos_vector_t* raw_trilinos_vec_ptr(){return vec_.get();}

    ///
    /// \brief get_vector Returns the raw pointer to the Epetra_MultiVector
    ///
    const trilinos_vector_t* raw_trilinos_vec_ptr()const{return vec_.get();}

    ///
    /// \brief Zero the entries
    ///
    void zero();

    ///
    /// \brief Initialize the multi-vector with \p n vectors.
    /// if \p fast is set to false we set the entries of the
    /// vector to zero. The default is to initialize to zero
    ///
    void init(uint_t n, bool fast=false);

    ///
    /// \brief Initialize the multi-vector with n vectors each of which has length m
    ///  and set all entries to val
    ///
    void init(uint_t n, uint_t m, real_t val);

    ///
    /// \brief Add val to the i-th entry of the n-th vector
    ///
    void add_entry(uint_t n, uint_t i,real_t val);

    ///
    /// \brief Set the i-th entry of the n-th vector  to val
    ///
    void set_entry(uint_t n, uint_t i, real_t val);

    ///
    /// \brief Scale the vector with the given factor
    ///
    void scale(real_t factor);

    ///
    /// \brief For the given global indices add the the given values
    ///
    void add(const std::vector<uint_t>& dofs, const std::vector<real_t>& vals);

    ///
    /// \brief Pointer to the array of doubles containing the
    /// local values of the ith vector in the multi-vector.
    ///
    real_t* const & operator[](uint_t i)const;

    ///
    /// \brief Pointer to the array of doubles containing the
    /// local values of the ith vector in the multi-vector.
    ///
    real_t*& operator[](uint_t i);

    ///
    /// \brief Returns the start of the vector
    ///
    //iterator begin();

    ///
    /// \brief Returns one past the end of the vector
    ///
    //iterator end();

private:

    ///
    /// \brief Pointer to the actual Epetra_Vector
    ///
    std::unique_ptr<Epetra_MultiVector> vec_;

    ///
    /// \brief Pointer to the map needed by the
    /// Epetra_Vector
    ///
    std::unique_ptr<Epetra_Map> epetra_map_;

    ///
    /// \brief The communicator we use.
    /// The Trilinos::Epetra_SerialComm has default ctor
    ///
    Epetra_SerialComm comm_;

};

}
}
#endif
#endif // TRILINOS_EPETRA_MULTIVECTOR_H

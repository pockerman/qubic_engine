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
namespace numerics{

/// forward declarations
class TrilinosKrylovSolver;

/// \brief Serial Trilinos Epetra Vector wrapper
class TrilinosEpetraVector{

public:

    /// \brief The data type the vector holds
    typedef real_t value_t;

    typedef value_t* iterator;

    /// \brief Constructor
    TrilinosEpetraVector();

    /// \brief How many elements the vector has
    uint_t size()const;

    /// \brief How many elements the vector has
    uint_t global_size()const{return size();}

    /// \brief Zero the entries
    void zero();

    /// \detailed initialize the vector with the given size n local and global
    /// sizes will be the same
    /// if \p fast is set to false we set the entries of the
    /// vector to zero. The default is to initialize to zero
    void init(uint_t n, bool fast=false);

    /// initialize the vector with n entries and set all entries to val
    void init(uint_t n, real_t val);

    /// \detailed add to the i-th entry of the vector  val
    void add(uint_t i,real_t val);

    /// \detailed set the i-th entry of the vector  val
    void set_entry(uint_t i, real_t val);

    /// \detailed scale the vector with the given factor
    void scale(real_t factor);

    /// \brief Compress the vector
    void compress();

    ///for the given global indices add the the given values
    void add(const std::vector<uint_t>& dofs, const std::vector<real_t>& vals);

    ///read access to the i-th entry of the vector
    real_t operator[](uint_t i)const;

    /// read/write access to the i-th entry of the vector
    real_t& operator[](uint_t i);

    /// \brief Returns the i-th entry of the vector
    real_t get(uint_t i)const{return (*this)[i];}

    /// get the start of the vector
    iterator begin();

    /// get one past the end of the vector
    iterator end();

    /// \brief Print the vector
    std::ostream& print(std::ostream& out)const;

protected:

    Epetra_Vector* get_vector(){return vec_.get();}

private:

    /// \brief
    std::unique_ptr<Epetra_Vector> vec_;

    /// \brief
    std::unique_ptr<Epetra_Map> epetra_map_;

    Epetra_CombineMode last_action_;

    /// \brief The communicator we use.
    /// The Trilinos::Epetra_SerialComm has default ctor
    Epetra_SerialComm comm_;

    friend class TrilinosKrylovSolver;
};


}

}
#endif
#endif // TRILINOS_EPETRA_VECTOR_H

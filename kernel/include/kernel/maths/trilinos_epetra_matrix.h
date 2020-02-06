#ifndef TRILINOS_EPETRA_MATRIX_H
#define TRILINOS_EPETRA_MATRIX_H

#include "kernel/base/config.h"

#ifdef USE_TRILINOS

#include "kernel/base/types.h"

// Trilinos includes
#include <Epetra_CrsMatrix.h>
#include <Epetra_CrsGraph.h>

#include <memory>
#include <vector>

namespace kernel{
namespace numerics{

class TrilinosEpetraMatrix
{

    typedef std::vector<real_t> RowEntries;
    typedef std::vector<uint_t> RowIndices;



     ///
     /// Constructor; initializes the matrix to
     /// be empty, without any structure, i.e.
     /// the matrix is not usable at all. This
     /// constructor is therefore only useful
     /// for matrices which are members of a
     /// class. All other matrices should be
     /// created at a point in the data flow
     /// where all necessary information is
     /// available.
     ///
     /// You have to initialize
     /// the matrix before usage with
     /// \p init(...).
     ///
     TrilinosEpetraMatrix ();

     /// \brief detailed dtor
     ~TrilinosEpetraMatrix();

    /// \brief Initialize a matrix with m global rows having nz non-zero entries per rows
    /// note this function creates a matrix that has the same number of global and local rows
    void init(uint_t m,uint_t nz);

    /// \brief Initialize by passing in the given sparsity pattern
    void init(const Epetra_CrsGraph& graph);

    /// \brief Zro the entries of the matrix
    void zero();

    /// \brief Clear the memory that has been allocated for the matrix
    void clear();

    ///
    /// \brief Set the (i,j) entry of the matrix to val
    ///
    void set_entry(uint_t i,uint_t j,real_t val);

    /// \brief set the entries in RowIndices to the values in RowEntries
    void set_row_entries(const RowIndices& indices,const RowEntries& entries)const;

    /// \brief add to the (i,j) entry of the matrix the value val
    void add_entry(uint_t i,uint_t j,real_t val);

    /// \brief Add to the entries in RowIndices the values in RowEntries of the given Epetra_CrsMatrix
    void add_row_entries(RowIndices& indices,RowEntries& entries);

    ///
    ///signal the underlying Epetra_FECrsMatrix that filling
    ///of the matrix is completed
    ///
    void fill_completed(){mat_->FillComplete();}

    /// \brief Returns true iff fill_completed has been called
    bool is_filled()const{return mat_->Filled();}

    /// \brief The number of rows the matrix has
    uint_t m () const;

    /// \brief The number of columns the matrix has
    uint_t n () const;

    /// \brief Print the matrix
    std::ostream& print(std::ostream& out)const;

private:


   /// \brief The  Epetra datatype to hold matrix entries.
   /// A pointer since Epetra_CrsMatrix does not have a default ctor
   std::unique_ptr<Epetra_CrsMatrix> mat_;

};

}
}
#endif

#endif // TRILINOS_EPETRA_MATRIX_H

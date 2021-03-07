#ifndef TRILINOS_EPETRA_MATRIX_H
#define TRILINOS_EPETRA_MATRIX_H

#include "kernel/base/config.h"

#ifdef USE_TRILINOS

#include "kernel/base/types.h"

#include <Epetra_CrsMatrix.h>
#include <Epetra_CrsGraph.h>
#include <Epetra_SerialComm.h>

#include <memory>
#include <vector>

namespace kernel{
namespace numerics{

/// forward declarations
class TrilinosKrylovSolver;

///
/// \brief The TrilinosEpetraMatrix class.
/// Wrapper to Trilinos::Epetra_CrsMatrix matrix class
///
class TrilinosEpetraMatrix
{

public:

    typedef real_t value_t;
    typedef std::vector<real_t> row_entries_t;
    typedef std::vector<trilinos_int_t> row_indices_t;

    ///
    /// \brief trilinos_matrix_t The type of the Trilinos matrix
    /// type the class wraps
    ///
    typedef Epetra_CrsMatrix trilinos_matrix_t;

    ///
    /// \brief Constructor. Initializes an empty matrix without any structure, i.e.
    /// the matrix is not usable at all. This constructor is therefore only useful
    /// for matrices which are members of a
    /// class. All other matrices should be created at a point in the data flow
    /// where all necessary information is
    /// available.
    ///
    /// You have to initialize
    /// the matrix before usage with
    /// \p init(...).
    ///
    TrilinosEpetraMatrix ();

    ///
    /// \brief TrilinosEpetraMatrix Create a square matrix
    /// mxm and nnz number of non-zeros per row
    ///
    TrilinosEpetraMatrix(uint m, uint nnz);

    ///
    /// \brief Destructor
    ///
    ~TrilinosEpetraMatrix();

    ///
    /// \brief Returns the (i, j) entry
    ///
    real_t entry(uint_t i,  uint_t j)const;

    ///
    /// \brief Returns true iff fill_completed has been called
    ///
    bool is_filled()const{return mat_->Filled();}

    ///
    /// \brief The number of rows the matrix has
    ///
    uint_t m () const;

    ///
    /// \brief The number of columns the matrix has
    ///
    uint_t n () const;

    ///
    /// \brief Print the matrix
    ///
    std::ostream& print(std::ostream& out)const;

    ///
    /// \brief get_matrix returns the raw pointer to the Epetra_CrsMatrix
    ///
    Epetra_CrsMatrix* get_matrix(){return mat_.get(); }

    ///
    /// \brief get_matrix returns the raw pointer to the Epetra_CrsMatrix
    ///
    const Epetra_CrsMatrix* get_matrix()const{return mat_.get(); }

    ///
    /// \brief get_row_entries. Get a copy of the row values
    ///
    row_entries_t get_row_entries(uint_t r)const;

    ///
    /// \brief compute_transpose. Compute the transpose of this
    /// matrix and store the result into mat
    ///
    void compute_transpose(TrilinosEpetraMatrix& mat)const;

    ///
    /// \brief Initialize a matrix with m global rows and n globals columns
    /// having nz non-zero entries per rows
    ///
    void init(uint_t m, uint_t n, uint_t nz);

    ///
    /// \brief Initialize by passing in the given sparsity pattern
    ///
    void init(const Epetra_CrsGraph& graph);

    ///
    /// \brief Zero the entries of the matrix
    ///
    void zero();

    ///
    /// \brief Set the entries of the row to value
    ///
    void set_row_entries(uint_t r, real_t val);

    ///
    /// \brief set_row_entries Set the entries for row r
    ///
    void set_row_entries(uint_t r, const row_entries_t& entries);

    ///
    /// \brief Set the entries in row_indices to the values in row_entries
    ///
    void set_row_entries(const row_indices_t& indices,const row_entries_t& entries)const;

    ///
    /// \brief Set the (i,j) entry of the matrix to val
    ///
    void set_entry(uint_t i,uint_t j,real_t val);

    ///
    /// \brief Add to the (i,j) entry of the matrix the value val
    ///
    void add_entry(uint_t i,uint_t j,real_t val);

    ///
    /// \brief Add to the entries in row_indices the
    /// values in row_entries of the given Epetra_CrsMatrix
    ///
    void add_row_entries(row_indices_t& indices, row_entries_t& entries);

    ///
    /// \brief Signal the underlying Epetra_FECrsMatrix that filling
    ///of the matrix is completed
    ///
    void fill_completed(){mat_->FillComplete();}


private:

   ///
   /// \brief The  Epetra datatype to hold matrix entries.
   /// A pointer since Epetra_CrsMatrix does not have a default constructor
   ///
   std::unique_ptr<Epetra_CrsMatrix> mat_;

   ///
   /// \brief The communicator we use.
   /// The Trilinos::Epetra_SerialComm has default constructor
   ///
   Epetra_SerialComm comm_;

   ///
   /// \brief Pointer to the map needed by the Epetra_CrsMatrix
   ///
   std::unique_ptr<Epetra_Map> epetra_map_;

};

}
}
#endif
#endif // TRILINOS_EPETRA_MATRIX_H

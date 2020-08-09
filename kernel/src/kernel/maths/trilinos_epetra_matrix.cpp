#include "kernel/maths/trilinos_epetra_matrix.h"

#ifdef USE_TRILINOS

#include <exception>
#include <algorithm>
#include <iostream>

namespace kernel{
namespace numerics{

TrilinosEpetraMatrix::TrilinosEpetraMatrix()
                           :
                           mat_(),
                           comm_(),
                           epetra_map_()
{}

TrilinosEpetraMatrix::~TrilinosEpetraMatrix()
{}

uint_t
TrilinosEpetraMatrix::m () const{

    if(! mat_){
        throw std::logic_error("Matrix pointer has not been initialized");
    }
    return mat_->NumGlobalRows();
}

uint_t
TrilinosEpetraMatrix::n() const{

    if(! mat_){
        throw std::logic_error("Matrix pointer has not been initialized");
    }

    return mat_->NumGlobalCols();
}

void
TrilinosEpetraMatrix::set_entry(uint_t i,uint_t j,real_t val){

    if(!mat_){
        throw std::logic_error("Matrix has not been initialized");
    }

    int epetra_i = static_cast<int>(i);
    int epetra_j = static_cast<int>(j);
    real_t epetra_value = val;

    //if we have constructed with graph
    //then InsertGlobalValues does not work
    //only Replace
    int success = 0;
    if(mat_->Filled()){

       success = mat_->ReplaceGlobalValues(epetra_i, 1, &epetra_value, &epetra_j);
    }
    else{
       success = mat_->InsertGlobalValues (epetra_i, 1, &epetra_value, &epetra_j);
    }

    if(success != 0){
        throw std::logic_error("An error occured whilst setting the matrix entry ");
    }
}

real_t
TrilinosEpetraMatrix::entry(uint_t i,  uint_t j)const{

    if(!mat_){
        throw std::logic_error("Matrix has not been initialized");
    }

    // Extract local indices in
    // the matrix.
    int trilinos_i = mat_->GRID(static_cast<int>(i));
    int trilinos_j = mat_->GCID(static_cast<int>(j));

    // If the data is not on the
    // present processor, we can't
    // continue. Just print out zero
    if ((trilinos_i == -1 ) || (trilinos_j == -1)){
      return 0.;
    }

    real_t value = 0.0;

    // Prepare pointers for extraction
    // of a view of the row.
    int nnz_present = mat_->NumMyEntries(trilinos_i);
    int nnz_extracted;
    int *col_indices;
    real_t *values;

    // Generate the view and make
    // sure that we have not generated
    // an error.
    int ierr = mat_->ExtractMyRowView(trilinos_i, nnz_extracted, values, col_indices);

    if(ierr != 0){
        throw std::logic_error("An error occured whilst reading the matrix entry");
     }

    // Search the index where we
    // look for the value, and then
    // finally get it.
    int *el_find = std::find(col_indices, col_indices + nnz_present, trilinos_j);

    int local_col_index = (int)(el_find - col_indices);


    // This is actually the only
    // difference to the () function
    // querying (i,j), where we throw an
    // exception instead of just
    // returning zero for an element
    // that is not present in the
    // sparsity pattern.
    if (local_col_index != nnz_present){
          value = values[local_col_index];
    }

    return value;
}

void
TrilinosEpetraMatrix::set_row_entries(const TrilinosEpetraMatrix::row_indices_t& indices,
                                          const TrilinosEpetraMatrix::row_entries_t& entries)const{

    if(!mat_){
        throw std::logic_error("Matrix has not been initialized");
    }

    if(indices.size() != entries.size()){
        throw std::logic_error("sizes don't match");
    }

    //the first entry is the diagonal entry and the row index
    int row = static_cast<int>(indices[0]);
    int num_entries = static_cast<int>(indices.size());
    int success = 0;

    if(mat_->Filled())
    {
       success = mat_->ReplaceGlobalValues(row, num_entries, &entries[0], (int *)&indices[0]);
    }
    else
    {

      success = mat_->InsertGlobalValues(row, num_entries, &entries[0], (int*)&indices[0]);
    }

    if(success != 0){
        throw std::logic_error("An error occured whilst setting the matrix entry");
    }
}

void
TrilinosEpetraMatrix::set_row_entries(uint_t r, real_t val){

    if(!mat_){
        throw std::logic_error("Matrix has not been initialized");
    }

    int local_row = mat_->LRID(static_cast<int>(r));

    if(local_row>=0){

        real_t* values = nullptr;
        int* col_indices = nullptr;
        int num_entries;

        const int success = mat_->ExtractMyRowView(local_row,num_entries, values, col_indices);

        if(success != 0){
            throw std::logic_error("An error occured whilst setting the matrix row entries ");
        }

        int* diag_find = std::find(col_indices,col_indices+num_entries,local_row);
        int diag_index = (int)(diag_find-col_indices);

        for(int j=0; j<num_entries; ++j){
         if(diag_index != j )
            values[j] = val;
        }

        if(diag_find && val != 0.0){

           values[diag_index] = val;
         }
      }
}

void
TrilinosEpetraMatrix::set_row_entries(uint_t r, const TrilinosEpetraMatrix::row_entries_t& entries){

   //TrilinosEpetraMatrix::row_indices_t rows(entries.size(), r);
   //set_row_entries(rows, entries);

    if(!mat_){
        throw std::logic_error("Matrix has not been initialized");
    }

    int global_row = static_cast<int>(r);
    int num_entries = static_cast<int>(entries.size());
    TrilinosEpetraMatrix::row_indices_t columns(n(), 0);
    for(uint_t t=0; t<n(); ++t){
       columns[t] = t;
    }

    auto success = mat_->InsertGlobalValues(global_row, num_entries, &entries[0], (int*)&columns[0]);


    if(success != 0){
        throw std::logic_error("An error occured whilst setting matrix row entries");
    }
}

void
TrilinosEpetraMatrix::add_entry(uint_t i, uint_t j, real_t val){

    if(!mat_){
        throw std::logic_error("Matrix has not been initialized");
    }

    int epetra_i = static_cast<int>(i);
    int epetra_j = static_cast<int>(j);

    real_t epetra_value = val;

    //we need to determine if the (epetra_i,epetra_j) index exists
    //if it doesn't we need to call InsertGlobalValues

    const Epetra_CrsGraph& graph = mat_->Graph();

    int success = 0;
    int numIndices=0;
    int* indices = nullptr;

    graph.ExtractGlobalRowView(epetra_i, numIndices, indices);

    if(numIndices==0){
      success = mat_->InsertGlobalValues (epetra_i, 1, &epetra_value, &epetra_j);
    }
    else {


      //number of indices is not zero
      //attempt to find the column j

      for(int i=0; i<numIndices; ++i){

        if(indices[i] == epetra_j) {

           success = mat_->SumIntoGlobalValues(epetra_i, 1, &epetra_value, &epetra_j);

           if(success != 0){
               throw std::logic_error("An error occured whilst adding the matrix entry");
           }

          // we found the column so we can return
          return;
        }
      }

      success = mat_->InsertGlobalValues (epetra_i, 1, &epetra_value, &epetra_j);

      if(success != 0){
          throw std::logic_error("An error occured whilst adding the matrix entry");
      }

    }
}

void
TrilinosEpetraMatrix::add_row_entries(TrilinosEpetraMatrix::row_indices_t& indices,
                                      TrilinosEpetraMatrix::row_entries_t& entries){

    if(!mat_){
        throw std::logic_error("Matrix has not been initialized");
    }

    if(indices.size() != entries.size()){
        throw std::logic_error("sizes don't match");
    }


    //the first entry is the diagonal entry and the row index
    int row = static_cast<int>(indices[0]);
    int num_entries = static_cast<int>(indices.size());

    //we need to determine if the (epetra_i,epetra_j) index exists
    //if it doesn't we need to call InsertGlobalValues
    const Epetra_CrsGraph& graph = mat_->Graph();

    int success = 0;
    int numIndices=0;
    int* idxs_ptr = nullptr;

     success = graph.ExtractGlobalRowView(row, numIndices, idxs_ptr);

     if(success != 0){
         throw std::logic_error("An error occured whilst extracting global row view. Error code is: " + std::to_string(success));
     }

     if(numIndices == 0){

       if(mat_->IndicesAreLocal()){
           throw std::logic_error("Matrix indices should be not local");
       }

       if(mat_->IndicesAreContiguous()){
           throw std::logic_error("Matrix indices should be not contiguous");
       }

       /// Insert a list of elements in a given global row of the matrix.
       ///
       /// This method is used to construct a matrix for the first time.  It cannot
       /// be used if the matrix structure has already been fixed (via a call to FillComplete()).
       /// If multiple values are inserted for the same matrix entry, the values are initially
       /// stored separately, so memory use will grow as a result.  However, when FillComplete is called
       /// the values will be summed together and the additional memory will be released.
       ///
       /// For example, if the values 2.0, 3.0 and 4.0 are all inserted in Row 1, Column 2, extra storage
       /// is used to store each of the three values separately.  In this way, the insert process does not
       /// require any searching and can be faster.  However, when FillComplete() is called, the values
       /// will be summed together to equal 9.0 and only a single entry will remain in the matrix for
       /// Row 1, Column 2.
       ///
       /// \param GlobalRow - (In) Row number (in global coordinates) to put elements.
       /// \param NumEntries - (In) Number of entries.
       /// \param Values - (In) Values to enter.
       /// \param Indices - (In) Global column indices corresponding to values.
       ///
       /// \return Integer error code, set to 0 if successful. Note that if the
       /// allocated length of the row has to be expanded, a positive warning code
       /// will be returned.
       ///
       /// \warning This method may not be called once FillComplete() has been called.
       ///
       /// \pre IndicesAreLocal()==false && IndicesAreContiguous()==false
       ///
       ///
       success = mat_->InsertGlobalValues (row, num_entries, &entries[0], (int*)&indices[0]);

       if(success != 0){
           throw std::logic_error("An error occured whilst adding the matrix entry. Error code is: " + std::to_string(success));
       }

       return;
     }
     else{

        success = mat_->SumIntoGlobalValues(row, num_entries, &entries[0], (int *)&indices[0]);
        if(success != 0){
          throw std::logic_error("An error occured whilst adding the matrix entry. Error code is: " + std::to_string(success));
        }

        return;
     }
}

void TrilinosEpetraMatrix::init(uint_t m , uint_t n, uint_t nz){

    /// Epetra_Map constructor for a user-defined linear distribution of elements.
    /// Creates a map that puts NumMyElements on the calling processor. If
    ///   NumGlobalElements=-1, the number of global elements will be
    ///   the computed sum of NumMyElements across all processors in the
    ///   Epetra_Comm communicator.

    auto NumMyElements = static_cast<int>(m);
    auto NumGlobalElements = static_cast<int>(m);
    auto index_start = 0;
    epetra_map_.reset(new Epetra_Map(NumGlobalElements, NumMyElements,  index_start, comm_));

   //zero based (C-style) indexing
   //epetra_map_.reset(new Epetra_Map(NumGlobalElements, static_cast<int>(n), 0, comm_));

   //create the matrix
   mat_.reset(new Epetra_CrsMatrix (Copy, *epetra_map_.get(), static_cast<int>(nz)));

}


void
TrilinosEpetraMatrix::init(const Epetra_CrsGraph& graph)
{

  //create the matrix by passing in the Epetra_CrsGraph
  mat_.reset(new Epetra_CrsMatrix(Copy, graph));

}

void
TrilinosEpetraMatrix::zero(){

 if(! mat_){
     throw std::logic_error("Matrix pointer has not been initialized");
 }

  mat_->Scale(0.0);
}

std::ostream&
TrilinosEpetraMatrix::print(std::ostream& out)const{

  if(! mat_){
     throw std::logic_error("Matrix pointer has not been initialized");
  }

  mat_->Print(out);
  return out;
}
}
}
#endif

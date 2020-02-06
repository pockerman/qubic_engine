#include "kernel/maths/trilinos_epetra_matrix.h"

#ifdef USE_TRILINOS

namespace kernel{
namespace numerics{

TrilinosEpetraMatrix::TrilinosEpetraMatrix()
                           :
                           mat_()
{}

TrilinosEpetraMatrix::~TrilinosEpetraMatrix()
{}

uint_t
TrilinosEpetraMatrix::m () const{
 return mat_->NumGlobalRows();
}

uint_t
TrilinosEpetraMatrix::n() const{
 return mat_->NumGlobalCols();
}

void
TrilinosEpetraMatrix::set_entry(uint_t i,uint_t j,real_t val)
{

  policy_.set_entry(i,j,val,*mat_);

}

void
TrilinosEpetraMatrix::set_row_entries(const TrilinosEpetraMatrix::RowIndices& indices,
                                          const TrilinosEpetraMatrix::RowEntries& entries)const{

  policy_.set_row_entries(indices,entries,*mat_);

}

void
TrilinosEpetraMatrix::add_entry(uint_t i,uint_t j,real_t val){

  policy_.add_entry(i,j,val,*mat_);
}



void
TrilinosEpetraMatrix::add_row_entries(TrilinosEpetraMatrix::RowIndices& indices,
                                        typename TrilinosEpetraMatrix::RowEntries& entries)
{

  //AssertBiblSimPP(mat_!=nullptr,ExecNullPointer());
  policy_.add_row_entries(indices,entries,*mat_);

}

void TrilinosEpetraMatrix::init(uint_t m,uint_t nz)
{

   //clear the memory if needed
   clear();

   //ask the policy to create a map
   policy_.create_map(m, m);

   //create the matrix
   mat_ = new Epetra_CrsMatrix (Copy, *policy_.map(), static_cast<int>(nz));

}


void
TrilinosEpetraMatrix::init(const Epetra_CrsGraph& graph)
{

  //clear the memory if needed
  clear();

  //create the matrix by passing in the Epetra_CrsGraph
  mat_ = new Epetra_CrsMatrix (Copy, graph);

}

void
TrilinosEpetraMatrix::zero(){

  mat_->Scale(0.0);

}

std::ostream&
TrilinosEpetraMatrix::print(std::ostream& out)const{

  mat_->Print(out);
  return out;
}
}
}
#endif

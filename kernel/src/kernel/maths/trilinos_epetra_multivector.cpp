#include "kernel/maths/trilinos_epetra_multivector.h"

#ifdef USE_TRILINOS

#include <exception>

namespace kernel {
namespace algebra{


TrilinosEpetraMultiVector::TrilinosEpetraMultiVector()
    :
      vec_()
{}

TrilinosEpetraMultiVector::TrilinosEpetraMultiVector(uint_t n, uint_t m, real_t val)
    :
      vec_()
{
    init(n, m, val);
}

std::ostream&
TrilinosEpetraMultiVector::print(std::ostream& out)const{

    for(uint_t e=0; e<size(); ++e){

        out<<(*this)[e]<<std::endl;
    }

    return out;
}

uint_t
TrilinosEpetraMultiVector::size()const{

    if(!vec_){
        throw std::logic_error("Vector has not been initialized");
    }

    return vec_->MyLength();
}

uint_t
TrilinosEpetraMultiVector::n_vectors()const{

    if(!vec_){
        throw std::logic_error("Vector has not been initialized");
    }

    return vec_->NumVectors();
}

real_t
TrilinosEpetraMultiVector::get(uint n, uint_t i)const{

    if(!vec_){
        throw std::logic_error("Vector has not been initialized");
    }

    if(n >= n_vectors()){
        throw std::logic_error("Invalid vector index.");
    }

    if(i >= size()){
        throw std::logic_error("Invalid entry index.");
    }

    return vec_->operator[](static_cast<int>(n))[static_cast<int>(i)];

}


real_t* const &
TrilinosEpetraMultiVector::operator[](uint_t i)const{

    if(!vec_){
        throw std::logic_error("Vector has not been initialized");
    }

    if(i >= n_vectors()){
        throw std::logic_error("Invalid vector index.");
    }

    int idx = static_cast<int>(i);
    return vec_->operator[](idx);
}

real_t*&
TrilinosEpetraMultiVector::operator[](uint_t i){

    if(!vec_){
        throw std::logic_error("Vector has not been initialized");
    }

    if(i >= n_vectors()){
        throw std::logic_error("Invalid vector index.");
    }

    int idx = static_cast<int>(i);
    return vec_->operator[](idx);

}

/*real_t*
TrilinosEpetraMultiVector::begin(){

    if(!vec_){
        throw std::logic_error("Vector has not been initialized");
    }

    return &(vec_->operator[](0));
}

real_t*
TrilinosEpetraMultiVector::end(){

    if(!vec_){
        throw std::logic_error("Vector has not been initialized");
    }

    return &(vec_->operator[](vec_->MyLength()));
}*/




void
TrilinosEpetraMultiVector::zero(){

    if(!vec_){
        throw std::logic_error("Vector has not been initialized");
    }

   vec_->PutScalar(0.0);

}

void
TrilinosEpetraMultiVector::init(uint_t n, bool fast){

    // Epetra_Map constructor for a user-defined linear distribution of elements.
    // Creates a map that puts NumMyElements on the calling processor. If
    // NumGlobalElements=-1, the number of global elements will be
    // the computed sum of NumMyElements across all processors in the
    // Epetra_Comm communicator.

    auto NumMyElements = static_cast<int>(n);
    auto NumGlobalElements = static_cast<int>(n);
    auto index_start = 0;
    epetra_map_.reset(new Epetra_Map(NumGlobalElements, NumMyElements,  index_start, comm_));

    //reser the map that the vector holds
    vec_.reset(new Epetra_MultiVector(*epetra_map_.get(), n, fast));

    if(!fast){
        vec_->PutScalar(0.0);
    }

}

void
TrilinosEpetraMultiVector::init(uint_t n, uint_t m, real_t val){

    // Epetra_Map constructor for a user-defined linear distribution of elements.
    // Creates a map that puts NumMyElements on the calling processor. If
    // NumGlobalElements=-1, the number of global elements will be
    // the computed sum of NumMyElements across all processors in the
    // Epetra_Comm communicator.

    auto num_my_elements = static_cast<int>(m);
    auto num_global_elements = static_cast<int>(m);
    auto index_start = 0;
    epetra_map_.reset(new Epetra_Map(num_global_elements, num_my_elements,  index_start, comm_));

    // reset the vector pointer
    vec_.reset(new Epetra_MultiVector(*epetra_map_.get(), n, true));
    vec_->PutScalar(val);
}


void
TrilinosEpetraMultiVector::set_entry(uint_t n, uint_t i, real_t val){

    if(!vec_){
        throw std::logic_error("Vector has not been initialized");
    }

    if(i >= vec_->GlobalLength()){
        throw std::logic_error("Invalid index");
    }

    int idx = static_cast<int>(i);
    int v_idx = static_cast<int>(n);
    vec_->ReplaceGlobalValue(idx, v_idx, val);
}


void
TrilinosEpetraMultiVector::add_entry(uint_t n, uint_t i, real_t val){

    if(!vec_){
        throw std::logic_error("Vector has not been initialized");
    }

    if(i >= vec_->GlobalLength()){
        throw std::logic_error("Invalid index");
    }

    int idx = static_cast<int>(i);
    int v_idx = static_cast<int>(n);
    vec_->SumIntoGlobalValue(idx, v_idx, val);
}

void
TrilinosEpetraMultiVector::scale(real_t factor){

    if(!vec_){
        throw std::logic_error("Vector has not been initialized");
    }

    vec_->Scale(factor);
}





}
}

#endif

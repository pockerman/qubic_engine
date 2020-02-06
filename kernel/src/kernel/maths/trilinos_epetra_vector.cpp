#include "kernel/maths/trilinos_epetra_vector.h"

#include <exception>

namespace kernel{
namespace numerics{

TrilinosEpetraVector::TrilinosEpetraVector()
    :
      vec_(),
      last_action_()
{}


real_t
TrilinosEpetraVector::operator[](uint_t i)const{

    if(!vec_){
        throw std::logic_error("Vector has not been initialized");
    }

    int idx = static_cast<int>(i);
    return vec_->operator[](idx);
}

real_t&
TrilinosEpetraVector::operator[](uint_t i){

    if(!vec_){
        throw std::logic_error("Vector has not been initialized");
    }

    int idx = static_cast<int>(i);
    return vec_->operator[](idx);

}

real_t*
TrilinosEpetraVector::begin(){

    if(!vec_){
        throw std::logic_error("Vector has not been initialized");
    }

    return &(vec_->operator[](0));
}

real_t*
TrilinosEpetraVector::end(){

    if(!vec_){
        throw std::logic_error("Vector has not been initialized");
    }

    return &(vec_->operator[](vec_->MyLength()));
}

uint_t
TrilinosEpetraVector::size()const{

    if(!vec_){
        throw std::logic_error("Vector has not been initialized");
    }

    return vec_->MyLength();
}


void
TrilinosEpetraVector::zero(){

    if(!vec_){
        throw std::logic_error("Vector has not been initialized");
    }

   vec_->PutScalar(0.0);

}

void
TrilinosEpetraVector::init(uint_t n, bool fast){

    epetra_map_.reset(new Epetra_Map(static_cast<int>(n),static_cast<int>(n), 0, comm_));

    //reser the map that the vector holds
    vec_.reset(new Epetra_Vector(*epetra_map_.get()));

    if(!fast){
        vec_->PutScalar(0.0);
    }

}

void
TrilinosEpetraVector::init(uint_t n, real_t val){

    epetra_map_.reset(new Epetra_Map(static_cast<int>(n),static_cast<int>(n), 0, comm_));

    //reser the map that the vector holds
    vec_.reset(new Epetra_Vector(*epetra_map_.get()));
    vec_->PutScalar(val);
}



void
TrilinosEpetraVector::set_entry(uint_t i, real_t val){

    if(!vec_){
        throw std::logic_error("Vector has not been initialized");
    }

    if(i >= vec_->GlobalLength()){
        throw std::logic_error("Invalid index");
    }

    int idx = static_cast<int>(i);
    vec_->operator[](idx) = val;
}


void
TrilinosEpetraVector::add(uint_t i,real_t val){

    if(!vec_){
        throw std::logic_error("Vector has not been initialized");
    }

    if(i >= vec_->GlobalLength()){
        throw std::logic_error("Invalid index");
    }

    int idx = static_cast<int>(i);
    vec_->operator[](idx) += val;
}


void
TrilinosEpetraVector::scale(real_t factor){

    if(!vec_){
        throw std::logic_error("Vector has not been initialized");
    }

    vec_->Scale(factor);
}

void
TrilinosEpetraVector::compress(){

    //Select which mode to send to
    //Trilinos. Note that we use last_action
    //if available and ignore what the user
    //tells us to detect wrongly mixed
    //operations. Typically given_last_action
    //is only used on machines that do not
    //execute an operation (because they have
    //no own cells for example).
    Epetra_CombineMode mode = last_action_;

    if (last_action_ == Zero)
      {
        //if (given_last_action==::dealii::VectorOperation::add)
        //  mode = Add;
        //else if (given_last_action==::dealii::VectorOperation::insert)
          mode = Insert;
      }

    // Now pass over the information about
    // what we did last to the vector.
    //const int ierr = vec_->GlobalAssemble(mode);

    last_action_ = Zero;
}

}
}

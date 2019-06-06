#ifndef PREDICATES_H
#define PREDICATES_H

#include "numengine/base/types.h"

namespace numengine
{
//TODO: Review this implementation

    /**
    *\detailed base class to act as predicate
    */
   struct multi_predicate{};
   
  /**
    *\detailed base class for deriving various
    * class objects acting like predicates
    */
  template<typename ITERATOR>
  struct PredicateBase: public multi_predicate
  {
  
    PredicateBase()
        :
    multi_predicate(){}
    
    virtual bool operator()(const ITERATOR* /*itr*/)const
    {/*std::cout<<"Calling PredicateBase"<<std::endl;*/return true;}
  
  };
  
  template<typename ITERATOR>
  struct NotNull: public PredicateBase<ITERATOR>
  {
  
    NotNull()
        :
    PredicateBase<ITERATOR>(){}
    
    virtual bool operator()(const ITERATOR* itr)const
    {
      return (itr != nullptr );
    }
  
  };
  
  template<typename ITERATOR>
  struct OnProc:public PredicateBase<ITERATOR>
  {
  
    OnProc(size_type pid=0)
        :
    PredicateBase<ITERATOR>(),pid_(pid){}
    
    virtual bool operator()(const ITERATOR* itr)const
    {
      return (itr->pid()==pid_);
    }
    
    uint_t pid_;
  
  };
  
  
  template<typename ITERATOR>
  struct IsVertex: public PredicateBase<ITERATOR>
  {
  
    IsVertex()
        :
    PredicateBase<ITERATOR>(){}
    
    virtual bool operator()(const ITERATOR* itr)const
    {
      return itr->is_vertex();
    }
  
  };
  
  template<typename ITERATOR>
  struct IsActive: public PredicateBase<ITERATOR>
  {
  
    IsActive()
        :
    PredicateBase<ITERATOR>(){}
    
    virtual bool operator()(const ITERATOR* itr)const
    {
      return itr->is_active();
    }
  
  };

  template<typename ITERATOR>
  struct ActiveBoundaryObject
  {

     ActiveBoundaryObject(){}

     bool operator()(const ITERATOR* itr)const
     {
         return (itr->is_active() && itr->on_boundary());
     }
  };

}
#endif

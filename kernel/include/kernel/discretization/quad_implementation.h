#ifndef QUAD_IMPLEMENTATION_H
#define QUAD_IMPLEMENTATION_H

#include "kernel/base/types.h"
#include "kernel/base/kernel_consts.h"


#include<vector>

namespace kernel
{

namespace numerics
{

/**
  *\detailed dimension independent
  *implementation for quads
  */
  
 template<int dim>
 class QuadImpl
 {
 
 public:
 

    explicit QuadImpl(uint_t n_nodes_per_edge=KernelConsts::invalid_size_type())
                            :
                            n_nodes_per_edge_(n_nodes_per_edge)
                            {}

    uint_t n_nodes()const{return n_nodes_per_edge_;}
  

 private:
  
  /**
    * \detailed the number of nodes
    * per edge the quad has
    */
  uint_t n_nodes_per_edge_;
  
 
 };

}

}
#endif

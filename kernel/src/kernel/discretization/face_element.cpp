#include "kernel/discretization/face_element.h"
#include "kernel/discretization/element.h"
#include "kernel/discretization/face.h"
#include "kernel/discretization/node.h"
#include "kernel/base/kernel_consts.h"

namespace kernel{
namespace numerics{

template<int spacedim>
FaceElement<spacedim,0>::FaceElement()
                        :
                        GeomPoint<spacedim>(),
                        is_vertex_(false),
                        id_(KernelConsts::invalid_size_type())
                        {}
               
    
template<int spacedim>
FaceElement<spacedim,0>::FaceElement(uint_t global_id,
                                     real_t val,
                                     uint_t pid)
                                     :
                                     GeomPoint<spacedim>(val),
                                     is_vertex_(false)
                                     {}

template<int spacedim>
FaceElement<spacedim,0>::FaceElement(const std::array<real_t, spacedim>& coords,
                                     uint_t global_id,
                                     uint_t pid)
                                     :
                                     GeomPoint<spacedim>(coords),
                                     is_vertex_(false),
                                     id_(global_id)
                                     {}
                

template<int spacedim>
FaceElement<spacedim,0>::FaceElement(const GeomPoint<spacedim>& point, 
                                     uint_t global_id,
                                     uint_t pid)
                                     :
                                     GeomPoint<spacedim>(point),                                    
                                     is_vertex_(false),
                                     id_(global_id)
                                     {}
                
template<int spacedim>
FaceElement<spacedim,0>::FaceElement(const FaceElement<spacedim,0>& t)
                        :
                       GeomPoint<spacedim>(t.coordinates()),                       
                       is_vertex_(t.is_vertex_)
                       {}

template<int spacedim>
FaceElement<spacedim,0>& 
FaceElement<spacedim,0>::operator=(const FaceElement<spacedim,0>& o){

  if(this==&o)return *this;
  
  this->GeomPoint<spacedim>::operator=(o);
  is_vertex_ = o.is_vertex_;
  this->id_ = o.id_;
  
  return *this;

}

template<int spacedim>
real_t
FaceElement<spacedim,0>::owner_neighbor_distance()const{

    if(!owner_){
        throw std::logic_error("FaceElement owner has not been set");
    }

    if(!neighbor_){

        return owner_->centroid().distance(centroid());
    }

    return owner_->centroid().distance(neighbor_->centroid());
}


FaceElement<2,1>::FaceElement()
                        :
                        owner_(nullptr),
                        neighbor_(nullptr),
                        boundary_indicator_(KernelConsts::invalid_size_type()),
                        n_nodes_(2)
                        {}
                        
FaceElement<2,1>::FaceElement(uint_t id,uint_t pid)
                 :

                 owner_(nullptr),
                 neighbor_(nullptr),
                 boundary_indicator_(KernelConsts::invalid_size_type()),
                 n_nodes_(2)
                 {}

FaceElement<2,1>::FaceElement(uint_t id, uint_t nnodes, uint_t pid)
                 :
                 owner_(nullptr),
                 neighbor_(nullptr),
                 boundary_indicator_(KernelConsts::invalid_size_type()),
                 n_nodes_(nnodes)
                 {}
                 
std::ostream& 
FaceElement<2,1>::print_(std::ostream& out)const
{
  out<<" Boundary Indicator= "<<boundary_indicator_<<std::endl;
  
  if(owner_){
    out<<" Owner Id= "<<owner_->get_id()<<std::endl;
  }
  
  if(neighbor_){
    out<<" Neighbor Id= "<<neighbor_->get_id()<<std::endl;
  }
  
  return out;

}

void
FaceElement<2,1>::resize_nodes(){
    this->nodes_.resize(n_nodes_, nullptr);
}

FaceElement<2,1>::node_ptr_t
FaceElement<2,1>::node_ptr(uint_t n){
    if(n >= n_nodes()){
        throw std::logic_error("Invalid node index: " +
                               std::to_string(n) +
                               " not in [0," +
                               std::to_string(n_nodes()) +
                               ")");
    }

    return nodes_[n];
}

void
FaceElement<2,1>::set_node(uint_t n, FaceElement<2,1>::node_ptr_t node){

    if(n >= n_nodes()){
        throw std::logic_error("Invalid node index: " +
                               std::to_string(n) +
                               " not in [0," +
                               std::to_string(n_nodes()) +
                               ")");
    }

    if(this->nodes_.empty()){
        throw std::logic_error("Nodes list has not been initialized");
    }

    this->nodes_[n] = node;

}

std::vector<uint_t>
FaceElement<2,1>::get_vertices_ids()const{

    return {nodes_[0]->get_id(), nodes_[1]->get_id()};
}

real_t
FaceElement<2,1>::owner_neighbor_distance()const{

    if(!owner_){
        throw std::logic_error("FaceElement owner has not been set");
    }

    if(!neighbor_){

        return owner_->centroid().distance(centroid());
    }

    return owner_->centroid().distance(neighbor_->centroid());
}

real_t
FaceElement<2,1>::volume()const{

    if(this->nodes_.empty()){
        throw std::logic_error("Nodes list has not been initialized");
    }

    return (*nodes_[0]).distance(*nodes_[1]);
}

const GeomPoint<2>
FaceElement<2,1>::centroid()const{

    if(this->nodes_.empty()){
        throw std::logic_error("Nodes list has not been initialized");
    }

    real_t x = 0.5*((*this->nodes_[1])[0] +(*this->nodes_[0])[0]);
    real_t y = 0.5*((*this->nodes_[1])[1] + (*this->nodes_[0])[1]);
    std::vector<real_t> coords{x ,y};
    return GeomPoint<2>(coords);
}

FaceElement<3,1>::FaceElement()
                        :

                        owner_(nullptr),
                        neighbor_(nullptr),
                        boundary_indicator_(KernelConsts::invalid_size_type())
                        {}
                        
FaceElement<3,1>::FaceElement(uint_t id,uint_t pid)
                 :

                 owner_(nullptr),
                 neighbor_(nullptr),
                 boundary_indicator_(KernelConsts::invalid_size_type())
                 {}

FaceElement<3,1>::FaceElement(uint_t id, uint_t nnodes, uint_t pid)
    :
      owner_(nullptr),
      neighbor_(nullptr),
      boundary_indicator_(KernelConsts::invalid_size_type())
      {}
                 
std::ostream& 
FaceElement<3,1>::print_(std::ostream &out)const
{

  out<<" Boundary Indicator= "<<boundary_indicator_<<std::endl;
  
  if(owner_)
  out<<" Owner Id= "<<owner_->get_id()<<std::endl;
  
  if(neighbor_)
  out<<" Neighbor Id= "<<neighbor_->get_id()<<std::endl;
  
  return out;

}
                        

FaceElement<3,2>::FaceElement()
                 :

                 owner_(nullptr),
                 neighbor_(nullptr),
                 boundary_indicator_(KernelConsts::invalid_size_type())
                 {}
                 
FaceElement<3,2>::FaceElement(uint_t id,uint_t pid)
                 :

                 owner_(nullptr),
                 neighbor_(nullptr),
                 boundary_indicator_(KernelConsts::invalid_size_type())
                 {}
                 
std::ostream& 
FaceElement<3,2>::print_(std::ostream &out)const
{

  out<<" Boundary Indicator= "<<boundary_indicator_<<std::endl;
  
  if(owner_)
  out<<" Owner Id= "<<owner_->get_id()<<std::endl;
  
  if(neighbor_)
  out<<" Neighbor Id= "<<neighbor_->get_id()<<std::endl;
  
  return out;

}

template class FaceElement<1,0>;
template class FaceElement<2,0>;
template class FaceElement<3,0>;
}

}//libsimpp

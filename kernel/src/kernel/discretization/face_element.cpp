#include "kernel/discretization/face_element.h"
#include "kernel/discretization/element.h"
#include "kernel/discretization/face.h"
#include "kernel/base/kernel_consts.h"

namespace kernel
{

namespace numerics
{

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
FaceElement<spacedim,0>::operator=(const FaceElement<spacedim,0>& o)
{

  if(this==&o)return *this;
  
  this->GeomPoint<spacedim>::operator=(o);
  is_vertex_ = o.is_vertex_;
  this->id_ = o.id_;
  
  return *this;

}

/*template<int spacedim>
GeomPoint<spacedim>
FaceElement<spacedim,0>::coords()const{


}*/

FaceElement<1,0>::FaceElement()
                        :
                        GeomPoint<1>(),                       
                        is_vertex_(false),
                        boundary_indicator_(KernelConsts::invalid_size_type())
                        {}
               
    
//template<int spacedim>
FaceElement<1,0>::FaceElement(uint_t global_id,
                                     real_t val,
                                     uint_t pid)
                                     :
                                     GeomPoint<1>(val),
                                     is_vertex_(false),
                                     boundary_indicator_(KernelConsts::invalid_size_type())
                                     {}

//template<int spacedim>
FaceElement<1,0>::FaceElement(uint_t global_id,
                                     const std::vector<real_t>& data,
                                     uint_t pid)
                                     :
                                     GeomPoint<1>(data),                                    
                                     is_vertex_(false),
                                     boundary_indicator_(KernelConsts::invalid_size_type())
                                     {}
                
               
//template<int spacedim>
FaceElement<1,0>::FaceElement(const GeomPoint<1>& point, 
                                     uint_t global_id,
                                     uint_t pid)
                                     :
                                     GeomPoint<1>(point),

                                     is_vertex_(false),
                                     boundary_indicator_(KernelConsts::invalid_size_type())
                                     {}
                
//template<int spacedim>
FaceElement<1,0>::FaceElement(const FaceElement<1,0>& t)
                        :
                       GeomPoint<1>(t.coordinates()),                       
                       is_vertex_(t.is_vertex_),
                       boundary_indicator_(t.boundary_indicator_)
                       {}
                       
FaceElement<1,0>::~FaceElement()
{}

//template<int spacedim>
FaceElement<1,0>& 
FaceElement<1,0>::operator=(const FaceElement<1,0>& o)
{

  if(this==&o)return *this;
  
  this->GeomPoint<1>::operator=(o);
  is_vertex_ = o.is_vertex_;
  boundary_indicator_ = o.boundary_indicator_;
  
  return *this;

}


FaceElement<2,1>::FaceElement()
                        :
                        owner_(nullptr),
                        neighbor_(nullptr),
                        boundary_indicator_(KernelConsts::invalid_size_type())
                        {}
                        
FaceElement<2,1>::FaceElement(uint_t id,uint_t pid)
                 :

                 owner_(nullptr),
                 neighbor_(nullptr),
                 boundary_indicator_(KernelConsts::invalid_size_type())
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

#ifndef GEOM_POINT_H
#define GEOM_POINT_H

#include "parframe/base/types.h"

#include<vector>
#include<cmath>
#include<ostream>


namespace numengine
{

/**
  * A class that describes a point with spacedim spatial dimension space.
  */
  
  
template<int spacedim>
class GeomPoint
{

public:

	static const int dimension = spacedim;

  /**
    * ctor all dim data are assigned the given value
    */
  explicit GeomPoint(kernel::real_t val=static_cast<kernel::real_t>(0.0));

  /**
    * ctor construct a 2D point
    */
 GeomPoint(kernel::real_t x, kernel::real_t y);

 /**
    * ctor construct a 3D point
    */
 GeomPoint(kernel::real_t x, kernel::real_t y, kernel::real_t z);
  
  
  /**
    *\detailed create by passing a vector of data
    */
  explicit GeomPoint(const std::vector<kernel::real_t>& data);
  
  
  /**
    *\detailed create from the given array. \p data should have at least size spacedim
    * the first dim components are used
    */
  explicit GeomPoint(kernel::real_t data[]);
  
  
  /**
    * \detailed copy ctor
    */
  GeomPoint(const GeomPoint& t);
  
  
  /**
    *\detailed copy assignement operator
    */
  GeomPoint& operator=(const GeomPoint& t);
  
  
  /**
    *\detailed dtor
    */
 virtual ~GeomPoint(){}
  
// @}
  /**
   * @name Manipulators
   */
// @{


  /**
    * \detailed Add another vector, i.e. move
    * this point by the given
    * offset.
    */
    GeomPoint & operator += (const GeomPoint &);
    
  /**
    * \detailed Subtract another tensor.
    */
   GeomPoint & operator -= (const GeomPoint &);


   /**
     * negate the coordinates of the point
     */
   GeomPoint& operator-();
   
   
   /**
     * \detailed Scale the vector by
     * <tt>factor</tt>, i.e. multiply all
     * coordinates by <tt>factor</tt>.
     */
    GeomPoint & operator *= (kernel::real_t factor);
    
    
    /**
      * Scale the vector by <tt>1/factor</tt>.
      */
    GeomPoint & operator /= (kernel::real_t factor);
    
    
    /**
      *\detailed scale with a given factor
      */
    void scale(kernel::real_t factor);
    
    
    /**
      *\detailed scale this object by the given factors
      *\p factors should have size at least \p spacedim 
      */
    void scale(const std::vector<kernel::real_t>& factors);
    
    
    /**
      *\detailed zero the entries of the tensor
      */
    void zero();
    
    
    /**
      * add the coordinates of the given point to this
      * scaled by factor
      */
    void add_scaled(const GeomPoint& p, kernel::real_t factor);


   /**
      * add the coordinates of the given point to this
      * scaled by the given factors
      */
    void add_scaled(const GeomPoint& p, const std::vector<kernel::real_t>& factors);

// @}
  /**
   * @name Accessors
   */
// @{

    /**
      *\detailed access the i-th coordinate of the point
      */
    kernel::real_t& operator[](size_type i);
    
    
     /**
      *\detailed access the i-th coordinate of the point read-only
      */
    kernel::real_t operator[](size_type i)const;
    
     /**
      *\detailed access the i-th coordinate of the point read-only
      */
    kernel::real_t entry(size_type i)const{return (*this)[i];}
    
    /**
      * \detailed get a copy of the data of this object
      */
    std::vector<kernel::real_t> coordinates()const{return data_;}
    
    /**
      *\detailed get the max element in the point
      */
     kernel::real_t max()const;
     
     /**
       *\detailed get the min element in the point
       */
     kernel::real_t min()const;
     
     /**
       * get the distance from the given point
       */
     real_t distance(const GeomPoint&)const;

     /**
       * get the distance from the origin. This effectively
       * computes the L2 norm.
       */
        kernel::real_t L2_norm()const{return distance(GeomPoint(static_cast<kernel::real_t>(0.0));}
     
     /**
       * get a copy of this object
       */
     GeomPoint get_point_copy()const{return GeomPoint(data_);}

     /**
       * get the cross-product TODO: perhaps we should implement
       * this function as a free function since the cross product
       * is really defined in 3D only.
       */
     const GeomPoint<3> cross_product(const GeomPoint<3>& other)const;

     const GeomPoint<spacedim> unit()const;


     /** 
       *  compute the squared sum of the coordinates
       *  of this point
       */
     kernel::real_t squared_sum()const;
     
     
     /**
       * \detailed print the point
       */
     std::ostream& print_point_info(std::ostream &out)const;
        
private:

/**
  *\detailed hold the coordinates of the point
  */
    std::vector<kernel::real_t> data_;
};

 
  template<int spacedim>
  inline
  GeomPoint<spacedim>::GeomPoint(kernel::real_t val)
                  :
                  data_(spacedim,val)
                  {}

  template<int spacedim>
  inline
  GeomPoint<spacedim>::GeomPoint(kernel::real_t x, kernel::real_t y)
                        :
                        data_(spacedim,biblsimpp_value_traits<T>::zero())
  {

   //AssertBiblSimPP(spacedim==2,ExecInvalidCtorCall());

   data_[0] = x;
   data_[1] = y;
  }


  template<int spacedim,typename T>
  inline
  GeomPoint<spacedim,T>::GeomPoint(T x,T y,T z)
                        :
                        data_(spacedim,biblsimpp_value_traits<T>::zero())
  {

   AssertBiblSimPP(spacedim==3,ExecInvalidCtorCall());

   data_[0] = x;
   data_[1] = y;
   data_[2] = z;
  }
  
  //TODO: change this ctor: constructing an object and then checking
  //if the sizes are correct is a waste of time
  template<int spacedim,typename T>
  inline
  GeomPoint<spacedim,T>::GeomPoint(const std::vector<T>& data)
                  :
                  data_(data)
                  {
                  
                    //make sure what we constructed is correct
                    AssertBiblSimPP(spacedim==data.size(),ExecSizeMismatch(spacedim,data.size()));
                  
                  }
  
  
 template<int spacedim,typename T>
 inline
 GeomPoint<spacedim,T>::GeomPoint(T data[])
                 :
                 data_(spacedim,biblsimpp_value_traits<T>::zero())
  {
  
     for(int i=0; i<spacedim; ++i)
     data_[i] = data[i];
  
  
  }
  
  
  template<int spacedim,typename T>
  inline
  GeomPoint<spacedim,T>::GeomPoint(const GeomPoint<spacedim,T>& t)
                  :
                  data_(t.data_)
                  {}
  
  
 template<int spacedim,typename T>
 inline
 GeomPoint<spacedim,T>& 
 GeomPoint<spacedim,T>::operator=(const GeomPoint<spacedim,T>& t)
 {
 
    if(this==&t) return *this;
    
    data_ = t.data_;
    
    return *this;
 
 }

 template<int spacedim,typename T>
 inline
 GeomPoint<spacedim,T>& 
 GeomPoint<spacedim,T>::operator-()
 {

    (*this) *= -biblsimpp_value_traits<T>::one();

    return *this;

 }
 
 template<int spacedim,typename T>
 inline
 T& 
 GeomPoint<spacedim,T>::operator[](size_type i)
 {
 
   AssertBiblSimPP(i<spacedim,ExecInvalidIndex(i,0,spacedim));
   
   return data_[i];
 
 }
    
    
 template<int spacedim,typename T>
 inline
 T 
 GeomPoint<spacedim,T>::operator[](size_type i)const
 {
 
   AssertBiblSimPP(i<spacedim,ExecInvalidIndex(i,0,spacedim));
   
   return data_[i];
 
 }
 
 template<int spacedim,typename T>
 inline
 GeomPoint<spacedim,T>& 
 GeomPoint<spacedim,T>::operator += (const GeomPoint<spacedim,T> & t)
 {
 
   for(int i=0; i<spacedim; ++i)
    data_[i] += t.data_[i];
    
    return *this;
 
 }
    
 template<int spacedim,typename T>
 inline
 GeomPoint<spacedim,T>& 
 GeomPoint<spacedim,T>::operator -= (const GeomPoint<spacedim,T> & t)
 {
 
   for(int i=0; i<spacedim; ++i)
    data_[i] -= t.data_[i];
    
    return *this;
 
 }
   
   
 template<int spacedim,typename T>
 inline
 GeomPoint<spacedim,T>& 
 GeomPoint<spacedim,T>::operator *= (T factor)
 {
 
   for(int i=0; i<spacedim; ++i)
    data_[i] *= factor;
    
   return *this;
 
 }
 
 template<int spacedim,typename T>
 inline
 GeomPoint<spacedim,T> & 
 GeomPoint<spacedim,T>::operator /= (T factor)
 {
 
   (*this)*=(static_cast<T>(1)/factor);
 
   return *this;
 }
 
 template<int spacedim,typename T>
 inline
 void 
 GeomPoint<spacedim,T>::add_scaled(const GeomPoint<spacedim,T>& p, T factor)
 {
 
   for(int i=0; i<spacedim; ++i)
    data_[i] += p.data_[i]*factor;
 
 }


 template<int spacedim,typename T>
 inline
 void 
 GeomPoint<spacedim,T>::add_scaled(const GeomPoint<spacedim,T>& p, const std::vector<T>& factors)
 {

   for(int i=0; i<spacedim; ++i)
    data_[i] += p.data_[i]*factors[i];

 } 
 
 
 template<int spacedim,typename T>
 inline
 T 
 GeomPoint<spacedim,T>::max()const
 {
 
   T max_val = data_[0];
   
   for(int i=1; i<spacedim; ++i)
   max_val = std::max(max_val,data_[i]);
   
   
   return max_val;
 
 }
     
 template<int spacedim,typename T>
 inline
 T 
 GeomPoint<spacedim,T>::min()const
 {
 
   T min_val = data_[0];
   
   for(int i=1; i<spacedim; ++i)
   min_val = std::min(min_val,data_[i]);
   
   
   return min_val;
 
 }
 
 
 template<int spacedim,typename T>
 inline
 T 
 GeomPoint<spacedim,T>::distance(const GeomPoint<spacedim,T>& p)const
 {
 
 
    T sum = biblsimpp_value_traits<T>::zero();
    
    std::vector<T> data = p.coordinates();
    
    for(int i=0; i<spacedim; ++i)
    sum += (data_[i]-data[i])*(data_[i]-data[i]);
    
    return std::sqrt(sum);
 
 
 }


 template<int spacedim,typename T>
 inline
 T 
 GeomPoint<spacedim,T>::squared_sum()const
 {

   T rslt = biblsimpp_value_traits<T>::zero();

   for(int i=0; i<spacedim; ++i)
    rslt += data_[i]*data_[i];

   return rslt;

 } 
 
   
 template<int spacedim,typename T>
 inline
 void 
 GeomPoint<spacedim,T>::scale(const std::vector<T>& factors)
 {
 
   AssertBiblSimPP(factors.empty()!=true,ExecUninitialized("\nThe Given std::vector Is Empty\n"));
   AssertBiblSimPP(factors.size()==spacedim,ExecSizeMismatch(spacedim,factors.size()));
   
   for(int i=0; i<spacedim; ++i)
   data_[i] *= factors[i];
 
 }
 
 template<int spacedim,typename T>
 inline
 void
 GeomPoint<spacedim,T>::zero()
 {
 
    for(int i=0; i<spacedim; ++i)
    data_[i] = biblsimpp_value_traits<T>::zero();
 
 }

 template<int spacedim,typename T>
 inline
 const GeomPoint<spacedim,T> 
 GeomPoint<spacedim,T>::unit()const
 {

   T length = distance(GeomPoint<spacedim,T>(biblsimpp_value_traits<T>::zero()));

   std::vector<T> vals(spacedim);

   for(int d=0; d<spacedim; ++d)
   vals[d] = data_[d]/length;


   return GeomPoint<spacedim,T>(vals);

 }
 
 template<int spacedim,typename T>
 inline
 std::ostream& 
 GeomPoint<spacedim,T>::print_point_info(std::ostream &out)const
 {
 
  out<<"( ";
  
  for(int i=0; i<spacedim; ++i)
  {
    out<<data_[i];
    
    if(i<spacedim-1)
    out<<",";
  } 
  
  out<<" )";
  
  return out;
 
 
 }

 template<int spacedim,typename T>
 inline
 const GeomPoint<3,T>
 GeomPoint<spacedim,T>::cross_product(const GeomPoint<spacedim,T>& other)const
 {


  // |     i          j          k    |
  // |(*this)(0) (*this)(1) (*this)(2)|
  // |   p(0)       p(1)       p(2)   |

   std::vector<T> vals(3);

   if(spacedim==3)
   {
     //from mathworld.wolfram.com/CrossProduct.html 
      vals[0] = data_[1]*other.data_[2] - data_[2]*other.data_[1];
      vals[1] = -data_[0]*other.data_[2] + data_[2]*other.data_[0];
      vals[2] = data_[0]*other.data_[1] - data_[1]*other.data_[0];
   }

  if(spacedim==2)
  {

    T zero = biblsimpp_value_traits<T>::zero();

    vals[0] = data_[1]*zero - zero*other.data_[1];
    vals[1] = -data_[0]*zero + zero*other.data_[0];
    vals[2] = data_[0]*other.data_[1] - data_[1]*other.data_[0];
  }
   
  return GeomPoint<3,T>(vals);
 }


 
//free functions that work on the GeomPoint<dim,spacedim,T> class
//these functions logically extend the interface of the class

/**
  *  \detailed Prints the values of this tensor in the
  *  form <tt>x1 x2 x3 etc</tt>.
  */
template<int spacedim,typename T>
inline
std::ostream & operator << (std::ostream &out, const GeomPoint<spacedim,T> &p)
{

 return p.print_point_info(out); 

}

/**
  *\detailed scale the given point by factor. This function does not
  * change the entries of \p t.
  */
template<int spacedim,typename T>
const GeomPoint<spacedim,T> scale(const GeomPoint<spacedim,T>& t, T factor)
{
  return (GeomPoint<spacedim,T>(t)*=factor);
}

/**
  *\detailed scale the given point by the factors in vector
  */
template<int spacedim,typename T>
const GeomPoint<spacedim,T> scale(const GeomPoint<spacedim,T>& t, const std::vector<T>& factors)
{

   std::vector<T> data = t.data();
  
  for(int i=0; i<spacedim; ++i)
   data[i] *= factors[i];

  return GeomPoint<spacedim,T>(data);

}

/**
  * \detailed get a point having the absolute values 
  * of the given point object
  */
template<int spacedim,typename T>
const GeomPoint<spacedim,T> abs(const GeomPoint<spacedim,T>& t)
{

  std::vector<T> data = t.coordinates();
  
  for(int i=0; i<spacedim; ++i)
  data[i] = std::abs(data[i]);
  
  return GeomPoint<spacedim,T>(data);

}

/**
  *\detailed allow multiplication from left with a factor. 
  */
  
template<int spacedim,typename T>
const GeomPoint<spacedim,T> operator*(T factor,const GeomPoint<spacedim,T>& t)
{

  return (GeomPoint<spacedim,T>(t)*=factor);

}

template<int spacedim,typename T>
const GeomPoint<spacedim,T> operator*(const GeomPoint<spacedim,T>& t,T factor)
{

 return (GeomPoint<spacedim,T>(t)*=factor);

}

template<int spacedim,typename T>
T operator*(const GeomPoint<spacedim,T>& p1, const GeomPoint<spacedim,T>& p2)
{


  T rslt = biblsimpp_value_traits<T>::zero();

  for(int d=0; d<spacedim; ++d)
   rslt += p1[d]*p2[d];

  return rslt;

}



template<int spacedim,typename T>
const GeomPoint<spacedim,T> operator/(const GeomPoint<spacedim,T>& t,T factor)
{

 return (GeomPoint<spacedim,T>(t)/=factor);

}

template<int spacedim,typename T>
const GeomPoint<spacedim,T> operator+(const GeomPoint<spacedim,T>& t1, const GeomPoint<spacedim,T>& t2)
{

return (GeomPoint<spacedim,T>(t1) += t2);

}

template<int spacedim,typename T>
const GeomPoint<spacedim,T> operator-(const GeomPoint<spacedim,T>& t1, const GeomPoint<spacedim,T>& t2)
{

return (GeomPoint<spacedim,T>(t1) -= t2);

}

template<int spacedim,typename T>
const GeomPoint<3,T> cross_product(const GeomPoint<spacedim,T>& p1,const GeomPoint<spacedim,T>& p2)
{

   std::vector<T> vals(3);

   if(spacedim==2)
   {



   } 


}

template<int spacedim,typename T>
bool operator==(const GeomPoint<spacedim,T>& t1, const GeomPoint<spacedim,T>& t2)
{


   bool result=true;
   
   for(int i=0; i<spacedim; ++i)
   if(t1[i] != t2[i]) result = false;


  return result;

}

template<int spacedim>
bool operator!=(const GeomPoint<spacedim,T>& t1, const GeomPoint<spacedim,T>& t2)
{

 return !(t1==t2);

}


}//libsimpp
#endif

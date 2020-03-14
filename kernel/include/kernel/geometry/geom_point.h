#ifndef GEOM_POINT_H
#define GEOM_POINT_H

#include "kernel/base/types.h"

#include <vector>
#include <array>
#include <cmath>
#include <ostream>
#include <algorithm>
#include <string>

namespace kernel{

/// A class that describes a point with spacedim spatial dimension space.
template<int spacedim, typename T=real_t>
class GeomPoint
{

public:

   typedef T value_type;
   static const int dimension = spacedim;

   /// \brief ctor all dim data are assigned the given value
   explicit GeomPoint(T val =  T());

   /// \brief Create by passing a vector of data
   template<typename Container>
   explicit GeomPoint(const Container& data);

   /// \brief Construct given an initializer_list
   explicit GeomPoint(const std::initializer_list<T>& list);

   /// \brief copy ctor
   GeomPoint(const GeomPoint& t);

   /// \brief copy assignement operator
   GeomPoint& operator=(const GeomPoint& t);

   /// \brief dtor
   virtual ~GeomPoint(){}

   /// \detailed Add another vector, i.e. move
   /// this point by the given
   /// offset.
   GeomPoint & operator += (const GeomPoint &);

   /// \detailed Subtract another tensor.
   GeomPoint & operator -= (const GeomPoint &);

   /// \detailed Scale the vector by
   /// <tt>factor</tt>, i.e. multiply all
   /// coordinates by <tt>factor</tt>.
   GeomPoint & operator *= (T factor);

   /// Scale the vector by <tt>1/factor</tt>.
   GeomPoint & operator /= (T factor);

   /// \detailed scale with a given factor
   void scale(T factor);

   /// \brief  scale this object by the given factors
   /// p factors should have size at least \p spacedim
   void scale(const std::vector<T>& factors);

   /// detailed zero the entries of the tensor
   void zero();

   /// \brief Add the coordinates of the given point to this scaled by factor
   void add_scaled(const GeomPoint& p, T factor);

   /// \brief Access the i-th coordinate of the point
   T& operator[](uint_t i);

   /// \brief  Access the i-th coordinate of the point read-only
   T operator[](uint_t i)const;

   /// \brief access the i-th coordinate of the point read-only
   T entry(uint_t i)const{return (*this)[i];}

   /// \brief Get a copy of the data of this object
   auto coordinates()const{return data_;}

   /// \brief Get the max element in the point
   T max()const;

   /// \brief Get the min element in the point
   T min()const;

   /// \brief Get the distance from the given point
   T distance(const GeomPoint&)const;

   /// \brief Return the distance from the origin
   T L2_norm()const{return distance(GeomPoint(static_cast<T>(0)));}

   /// \brief Returns the square sum of the compontents
   T square_sum()const;

   /// \brief print the point
   std::ostream& print_point_info(std::ostream &out)const;

   /// \brief Returns s string representation of the point
   const std::string to_string()const;
  
private:

    /// \brief Hold the coordinates of the point
    std::array<T, spacedim> data_;

};

template<int spacedim,typename T>
inline
GeomPoint<spacedim,T>::GeomPoint(T val)
:
data_()
{
    std::for_each(data_.begin(), data_.end(),  [=](T& item){item = val;});
}

template<int spacedim,typename T>
template<typename Container>
inline
GeomPoint<spacedim,T>::GeomPoint(const Container& data)
 :
data_()
{
    for(uint_t i=0; i<data_.size(); ++i){
        data_[i] = data[i];
    }
}

template<int spacedim,typename T>
GeomPoint<spacedim,T>::GeomPoint(const std::initializer_list<T>& list)
    :
      data_()
{
    if(list.size() != spacedim){
        throw std::logic_error("Invalid initialization list size");
    }

    auto start = list.begin();
    auto end = list.end();

    uint_t i = 0;
    for(; start != end; ++start){
        data_[i++] = *start;
    }
    /*for(uint_t i=0; i<data_.size(); ++i){
        data_[i] = list[i];
    }*/
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
GeomPoint<spacedim,T>::operator=(const GeomPoint<spacedim, T>& t){

   if(this==&t) {
       return *this;
   }
   data_ = t.data_;
   return *this;
}

template<int spacedim,typename T>
inline
T&
GeomPoint<spacedim,T>::operator[](uint_t i){
  return data_[i];
}


template<int spacedim,typename T>
inline
T
GeomPoint<spacedim,T>::operator[](uint_t i)const{
  return data_[i];
}

template<int spacedim,typename T>
inline
GeomPoint<spacedim,T>&
GeomPoint<spacedim,T>::operator += (const GeomPoint<spacedim,T> & t){

  for(int i=0; i<spacedim; ++i){
    data_[i] += t.data_[i];
  }
   return *this;
}

template<int spacedim,typename T>
inline
GeomPoint<spacedim,T>&
GeomPoint<spacedim,T>::operator -= (const GeomPoint<spacedim,T> & t){

  for(int i=0; i<spacedim; ++i){
    data_[i] -= t.data_[i];
  }
   return *this;
}

template<int spacedim,typename T>
inline
GeomPoint<spacedim,T>&
GeomPoint<spacedim,T>::operator *= (T factor){

  for(int i=0; i<spacedim; ++i){
   data_[i] *= factor;
  }

  return *this;
}

template<int spacedim,typename T>
inline
GeomPoint<spacedim,T> &
GeomPoint<spacedim,T>::operator /= (T factor){

  (*this) *= (static_cast<T>(1)/factor);
  return *this;
}

template<int spacedim,typename T>
inline
void
GeomPoint<spacedim,T>::add_scaled(const GeomPoint<spacedim,T>& p, T factor){

  for(int i=0; i<spacedim; ++i){
    data_[i] += p.data_[i]*factor;
  }
}

template<int spacedim,typename T>
T
GeomPoint<spacedim,T>::max()const{

  T max_val = data_[0];
  for(int i=1; i < spacedim; ++i){
       max_val = std::max(max_val,data_[i]);
  }

  return max_val;
}

template<int spacedim,typename T>
T
GeomPoint<spacedim,T>::min()const{

  T min_val = data_[0];

  for(int i=1; i<spacedim; ++i){
       min_val = std::min(min_val,data_[i]);
  }

  return min_val;
}

template<int spacedim,typename T>
T
GeomPoint<spacedim,T>::distance(const GeomPoint<spacedim,T>& p)const{

   T sum = T();

   auto data = p.coordinates();

   for(int i=0; i<spacedim; ++i)
   sum += (data_[i]-data[i])*(data_[i]-data[i]);

   return std::sqrt(sum);

}

template<int spacedim,typename T>
void
GeomPoint<spacedim,T>::scale(const std::vector<T>& factors){
  for(int i=0; i<spacedim; ++i){
        data_[i] *= factors[i];
    }
}

template<int spacedim,typename T>
T
GeomPoint<spacedim,T>::square_sum()const{
    T result = T(0);

    std::for_each(data_.begin(), data_.end(),
                  [&](const T& value){
        result += value*value;
    });

    return result;
}

template<int spacedim,typename T>
inline
void
GeomPoint<spacedim,T>::zero(){
   for(int i=0; i<spacedim; ++i)
        data_[i] = T();
}

template<int spacedim,typename T>
inline
std::ostream&
GeomPoint<spacedim,T>::print_point_info(std::ostream &out)const
{

 out<<"( ";

 for(int i=0; i<spacedim; ++i){
   out<<data_[i];

   if(i<spacedim-1){
        out<<",";
   }
 }

 out<<" )";
 return out;
}

template<int spacedim,typename T>
const std::string
GeomPoint<spacedim,T>::to_string()const{

    std::string rslt("");
    auto end = data_.begin();
    std::advance(end, spacedim-1);
    std::for_each(data_.begin(), end,
                  [&](const T& value){
        rslt += std::to_string(value);
        rslt +=",";
    });

    rslt  += std::to_string(data_[spacedim - 1]);
    return rslt;
}

//free functions that work on the GeomPoint<spacedim,T> class
//these functions logically extend the interface of the class

/**
  *  \detailed Prints the values of this tensor in the
  *  form <tt>x1 x2 x3 etc</tt>.
  */
template<int spacedim,typename T>
inline
std::ostream & operator << (std::ostream &out, const GeomPoint<spacedim,T> &p){
 return p.print_point_info(out);

}

/**
  *\detailed scale the given point by factor. This function does not
  * change the entries of \p t.
  */
template<int spacedim,typename T>
const GeomPoint<spacedim,T> scale(const GeomPoint<spacedim,T>& t, T factor){
  return (GeomPoint<spacedim,T>(t)*=factor);
}

/**
  *\detailed scale the given point by the factors in vector
  */
template<int spacedim,typename T>
const GeomPoint<spacedim,T> scale(const GeomPoint<spacedim,T>& t, const std::vector<T>& factors){

   auto data = t.data();

  for(int i=0; i<spacedim; ++i)
    data[i] *= factors[i];

  return GeomPoint<spacedim,T>(data);
}

/// \brief Return a point having the absolute values
/// of the given point object
template<int spacedim,typename T>
const GeomPoint<spacedim,T> abs(const GeomPoint<spacedim,T>& t){
  auto data = t.coordinates();

  for(int i=0; i<spacedim; ++i)
        data[i] = std::abs(data[i]);

  return GeomPoint<spacedim,T>(data);
}

/// Allow multiplication from left with a factor.
template<int spacedim,typename T>
const GeomPoint<spacedim,T> operator*(T factor,const GeomPoint<spacedim,T>& t){

  return (GeomPoint<spacedim,T>(t)*=factor);
}

template<int spacedim,typename T>
const GeomPoint<spacedim,T> operator*(const GeomPoint<spacedim,T>& t,T factor){

 return (GeomPoint<spacedim,T>(t)*=factor);
}

template<int spacedim,typename T>
const GeomPoint<spacedim,T> operator/(const GeomPoint<spacedim,T>& t,T factor){

 return (GeomPoint<spacedim,T>(t)/=factor);
}

template<int spacedim,typename T>
const GeomPoint<spacedim,T> operator+(const GeomPoint<spacedim,T>& t1, const GeomPoint<spacedim,T>& t2){

return (GeomPoint<spacedim,T>(t1) += t2);

}

template<int spacedim, typename T>
const GeomPoint<spacedim, T> operator-(const GeomPoint<spacedim,T>& t1, const GeomPoint<spacedim,T>& t2){
return (GeomPoint<spacedim,T>(t1) -= t2);
}

template<int spacedim, typename T>
bool operator==(const GeomPoint<spacedim,T>& t1, const GeomPoint<spacedim,T>& t2){
   bool result=true;

   for(int i=0; i<spacedim; ++i){
    if(t1[i] != t2[i]){

        result = false;
    }
   }

  return result;
}

template<int spacedim,typename T>
bool operator!=(const GeomPoint<spacedim,T>& t1, const GeomPoint<spacedim,T>& t2){
 return !(t1==t2);
}

}

#endif // GEOM_POINT_H

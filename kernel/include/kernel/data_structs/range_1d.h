#ifndef RANGE_1D_H
#define RANGE_1D_H

#include "kernel/base/types.h" //application types
#include <vector>
 
namespace kernel
{

	/// \brief struct that represents an 1d range of the form [first_,second)  similar to STL.     
	template<typename T>
	class range1d
	{
		
		  public:
		      
		      typedef T value_type;

					/// \brief Default constructor
					range1d()
					:
					first_(),
					second_(),
					grain_size_(static_cast<uint_t>(1))
					{}

					/// \brief Constructor. Create an object by passing the
					/// the [start,end) paramters. Optionally we can pass the
					/// grain size to be used when calling split()
		      range1d(T first, T second,
		              uint_t g = static_cast<uint_t>(1))
		              :
		              first_(first),
		              second_(second),
		              grain_size_(g)
		      {}
		      
		      /// \breif copy constructor. Construct a range from another range 
		      range1d(const range1d<T>& o)
		             :
		             first_(o.first_),
		             second_(o.second_),
		             grain_size_(o.grain_size_)
		      {}

					/// \brief Set the start/end for this range
					void set_range(T b,T e, uint_t grain_size = 1);

					/// \brief copy assignement operator
					range1d<T>& operator=(const range1d<T>& o);
		       
		      /// \brief Return the grain size for the 1d  range
		      uint_t grain_size()const{return grain_size_;}
		      
		      /// \brief Return the size (this->second_ - this->first_) of the range 
		      uint_t size()const{return second_ - first_;}
		      
		      /// \brief Return true if the range can be further divided
		      bool is_divisible()const{return size()>grain_size();}
		       
		      /// \brief Returns the inclusive beginning of the range
		      T begin()const{return first_;}
		      
		      /// \breif Return the exclusive end of the range
		      T end()const{return second_;}
		      
		      /// \brief Split the range into two. 
					/// If the range is not splitable meaning when is_divisible() returns false then it returns silently
		      range1d<T> split();

              /// \brief serialize the object into the given archive
              template<typename Archive>
              void serialize(Archive& arch, uint_t fileversion);

              /// \brief Overload operator(). Compatible for when using boost.mpi
              /// to transmit this type over the processes.
              range1d<T> operator()(const range1d<T>& in, const range1d<T>& out);
		
		  private:
		      
		      T first_;
		      T second_; 
		      uint_t grain_size_;   
	};


	template<typename T>
	inline
	void
	range1d<T>::set_range(T b,T e, uint_t grain_size){
		first_ = b;
		second_ = e;
		grain_size_ = grain_size;
	}

	template<typename T>
	inline
	range1d<T>
	range1d<T>::split(){
		  
		  if(!is_divisible()){
		      return range1d<T>(*this);
		  }
		  
		  T second_tmp_ = second_;
		  second_ = first_+(second_-first_)/2;
		  return range1d<T>(second_, second_tmp_, grain_size_);  
	}

	template<typename T>
	inline
	range1d<T>& 
	range1d<T>::operator=(const range1d<T>& o){

		if(this == &o)
			return *this;

		first_ = o.first_;
		second_ = o.second_;
		grain_size_ = o.grain_size_;
		return *this;
	}

	template<typename T>
	template<typename Archive>
	inline
	void 
	range1d<T>::serialize(Archive& arch, uint_t fileversion){
		arch & first_;
		arch & second_;
		arch & grain_size_;
	}

	template<typename T>
	inline
	range1d<T> 
	range1d<T>::operator()(const range1d<T>& in,const range1d<T>& out){

		return range1d(in.begin(),in.end(),in.grain_size());
	}

namespace partitioners
{
    /// \brief Given a range of integers partition it into nparts
    /// If the division difference/nparts, where difference = end-star, has a remainder then the
    /// last partition will be assigned these extra elements
    std::vector<range1d<uint_t>> partition_range_1d(const uint_t start, const uint_t end, const uint_t nparts);
}


}

#endif

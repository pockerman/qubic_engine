#ifndef RESULT_HOLDER_H
#define RESULT_HOLDER_H

/**
  * Utility class that simply holds the result of
  * a computation along side with a flag that indicates whether
  * the result is valid or not
  */

#include "kernel/base/config.h"
#include "kernel/base/types.h"
#include <boost/noncopyable.hpp>

#include <thread>
#include <utility>

namespace kernel
{

template<typename T>
class ResultHolder
{

public:

    typedef T value_type;
    typedef std::pair<value_type*, bool> result_type;

    /// \brief Constructor
    explicit ResultHolder(bool valid=false);

    /// \brief Constructor. Initialize the result
    explicit ResultHolder(value_type&& init, bool valid=false);

    /// \brief Copy constructor
    ResultHolder(const ResultHolder<T>& other);

    /// \brief  Copy assignement
    ResultHolder<T>& operator=(const ResultHolder<T>& other);

    /// \brief Move Copy constructor
    ResultHolder(ResultHolder<T>&& other)noexcept;

    /// \brief Move Copy assignement
    ResultHolder<T>& operator=(ResultHolder<T>&& other)noexcept;

    /// \brief Add factor to the result
    template<typename U>
    ResultHolder<T>& operator += (const U& factor);

    /// \brief Add the other result to this result
    /// if other is invalid the throws exception
    ResultHolder<T>& operator += (const ResultHolder<T>& other);

    /// \brief Subtract factor from the result
    template<typename U>
    ResultHolder<T>& operator -= (const U& factor);

    /// \brief Subract the other result from this result
    /// if other is invalid the throws exception
    ResultHolder<T>& operator -= (const ResultHolder<T>& other);

    /// \brief multiply by factor the result
    template<typename U>
    ResultHolder<T>& operator *= (const U& factor);

    /// \brief Multiply the other result to this result
    /// if other is invalid the throws exception
    ResultHolder<T>& operator *= (const ResultHolder<T>& other);

    /// \brief Divide by factor the result
    template<typename U>
    ResultHolder<T>& operator /= ( const U& factor);

    /// \brief Divide this result byt the other result
    /// if other is invalid it throws exception
    ResultHolder<T>& operator /= (const ResultHolder<T>& other);

    /// \brief Query whether the held result is valid
    bool is_result_valid()const{return valid_result_;}

    /// \brief Validate the result
    void validate_result(){valid_result_ = true;}

    /// \brief Invalidate the result. If reinit is true
    /// the underlying item is reinitialized using the default constructor
    void invalidate_result(bool reinit);

    /// \brief Get a copy of the internals
    void get_copy(ResultHolder<T>& other)const;

    /// \brief busy wait for the thread that calls it until the
    /// result becomes valid
    result_type get()const;

    /// \brief Attempt to get the result only if it is valid. It yields the calling thread
    /// as long as  the result is not valid
    result_type get_or_wait()const;

    /// \brief Attempt to get the result. If the result is not valid is waits for the
    /// specified time in milliseconds. It then returns the result regardless of its validity
    result_type get_or_wait_for(uint_t milliseconds)const;

    /// \brief Raw access to the resource
    value_type& get_resource(){return item_; }

    /// \brief Raw access to the resource
    const value_type& get_resource()const{return item_; }

private:

    /// \brief The resource to hold
    value_type item_;

    /// \brief flag indicating whether the result is valid
    bool valid_result_;
};

template<typename T >
ResultHolder<T>::ResultHolder(bool valid)
    :
   item_(),
   valid_result_(valid)
{}

template<typename T>
ResultHolder<T>::ResultHolder(T&& init, bool valid)
    :
   item_(init),
   valid_result_(valid)
{}

template<typename T>
ResultHolder<T>::ResultHolder(const ResultHolder<T>& other)
    :
      item_(other.item_),
      valid_result_(other.valid_result_)
{}

template<typename T>
ResultHolder<T>&
ResultHolder<T>::operator=(const ResultHolder<T>& other){

    if(this == &other){
        return *this;
    }

    item_ = other.item_;
    valid_result_ = other.valid_result_;
    other.invalidate_result(true);
    return *this;
}

template<typename T>
ResultHolder<T>::ResultHolder(ResultHolder<T>&& other)noexcept
    :
      item_(other.item_),
      valid_result_(other.valid_result_)
{
    other.invalidate_result(true);
}

template<typename T>
ResultHolder<T>&
ResultHolder<T>::operator=(ResultHolder<T>&& other)noexcept{

    if(this == &other){
        return *this;
    }

    item_ = other.item_;
    valid_result_ = other.valid_result_;
    other.invalidate_result(true);
    return *this;
}


template<typename T>
template<typename U>
ResultHolder<T>&
ResultHolder<T>::operator += (const U& factor){

   get_resource() += factor;
   return *this;
}

template<typename T>
ResultHolder<T>&
ResultHolder<T>::operator += (const ResultHolder<T>& other){

    if(!other.is_result_valid()){
        throw std::invalid_argument("Other result is not valid");
    }

    get_resource() += other.get_resource();
    return *this;
}

template<typename T>
template<typename U>
ResultHolder<T>&
ResultHolder<T>::operator -= (const U& factor){

   get_resource() -= factor;
   return *this;
}

template<typename T>
ResultHolder<T>&
ResultHolder<T>::operator -= (const ResultHolder<T>& other){

    if(!other.is_result_valid()){
        throw std::invalid_argument("Other result is not valid");
    }

    get_resource() -= other.get_resource();
    return *this;
}

template<typename T>
template<typename U>
ResultHolder<T>&
ResultHolder<T>::operator *= (const U& factor){

   get_resource() *= factor;
   return *this;
}

template<typename T>
ResultHolder<T>&
ResultHolder<T>::operator *= (const ResultHolder<T>& other){

    if(!other.is_result_valid()){
        throw std::invalid_argument("Other result is not valid");
    }

    get_resource() *= other.get_resource();
    return *this;
}

template<typename T>
template<typename U>
ResultHolder<T>&
ResultHolder<T>::operator /= (const U& factor){

    get_resource() /= factor;
    return *this;
}

template<typename T>
ResultHolder<T>&
ResultHolder<T>::operator /= (const ResultHolder<T>& other){

    if(!other.is_result_valid()){
        throw std::invalid_argument("Other result is not valid");
    }

    get_resource() /= other.get_resource();
    return *this;
}

template<typename T>
void
ResultHolder<T>::get_copy(ResultHolder<T>& other)const
{

    other.valid_result_ = valid_result_;
    other.item_ = item_;
}

template<typename T>
typename ResultHolder<T>::result_type
ResultHolder<T>::get()const{
    return std::make_pair(&(const_cast<ResultHolder<T>&>(*this).item_), valid_result_);
}

template<typename T>
typename ResultHolder<T>::result_type
ResultHolder<T>::get_or_wait()const{

    while(!valid_result_){
        std::this_thread::yield();
    }

    return std::make_pair(&(const_cast<ResultHolder<T>&>(*this).item_), valid_result_);
}


template<typename T>
typename ResultHolder<T>::result_type
ResultHolder<T>::get_or_wait_for(uint_t mills)const{

    if(!valid_result_){
        std::this_thread::sleep_for(std::chrono::milliseconds(mills));
    }

    return std::make_pair(&(const_cast<ResultHolder<T>&>(*this).item_), valid_result_);
}

template<typename T>
void
ResultHolder<T>::invalidate_result(bool reinit){

    if(reinit){
       item_ = typename ResultHolder<T>::value_type();
    }

    valid_result_ = false;
}


/**
 * @brief class ResultHolder<void>. Specialization that does not hold a
 * result. It simply signals if the execution was successful or not
 */
template<>
class ResultHolder<void>
{

public:

    typedef void value_type;
    typedef std::pair<void*, bool> result_type;

    /// \brief Constructor
    explicit ResultHolder(bool valid=false);

    /// \brief Query whether the held result is valid
    bool is_result_valid()const{return valid_result_;}

    /// \brief Validate the result
    void validate_result(){valid_result_ = true;}

    /// \brief Invalidate the result. If reinit is true
    /// the underlying item is reinitialized using the default constructor
    void invalidate_result(){valid_result_ = false;}

    /// \brief Get a copy of the internals
    void get_copy(ResultHolder<void>& other)const;

    /// \brief busy wait for the thread that calls it until the
    /// result becomes valid
    result_type get()const;

    /// \brief Attempt to get the result only if it is valid. It yields the calling thread
    /// as long as  the result is not valid
    result_type get_or_wait()const;

    /// \brief Attempt to get the result. If the result is not valid is waits for the
    /// specified time in milliseconds. It then returns the result regardless of its validity
    result_type get_or_wait_for(uint_t milliseconds)const;

private:

    /// \brief The resource to hold
    void* item_;

    /// \brief flag indicating whether the result is valid
    bool valid_result_;
};



///
/// \brief With Null type is the same as with void type
///
template<>
class ResultHolder<Null>: public ResultHolder<void>
{
public:



};



}

#endif // RESULT_HOLDER_H

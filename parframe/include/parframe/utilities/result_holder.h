#ifndef RESULT_HOLDER_H
#define RESULT_HOLDER_H

/**
  * Utility class that simply holds the result of
  * a computation along side with a flag that indicates whether
  * the result is valid or not
  */

#include "parframe/base/types.h"
#include <boost/noncopyable.hpp>

#include <thread>
#include <utility>

namespace kernel
{

template<typename T>
class ResultHolder: private boost::noncopyable
{

public:

    typedef T value_type;
    typedef std::pair<value_type*, bool> result_type;

    /// \brief Constructor
    explicit ResultHolder(bool valid=false);

    /// \brief Constructor
    explicit ResultHolder(value_type&& init, bool valid=false);

    /// \brief Overload operator!
    bool operator!()const{ return valid_result_;}

    /// \brief Query whether the held result is valid
    bool is_result_valid()const{return valid_result_;}

    /// \brief Validate the result
    void validate_result(){valid_result_ = true;}

    /// \brief Invalidate the result
    void invalidate_result(){valid_result_ = false;}

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
   item_(),
   valid_result_(valid)
{
    item_ = std::move(init);
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

}

#endif // RESULT_HOLDER_H

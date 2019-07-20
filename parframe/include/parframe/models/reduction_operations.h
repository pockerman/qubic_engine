#ifndef REDUCTION_OPERATIONS_H
#define REDUCTION_OPERATIONS_H

#include <thread>
#include <utility>

namespace kernel
{

template<typename T>
class Sum
{

public:

    typedef T value_type;
    typedef std::pair<value_type, bool> result_type;

    /// \brief Join the given value to the given result
    static void local_join(const value_type& val, value_type& rslt){rslt += val;}

    Sum()
        :
          result_(),
          valid_result_(false)
    {}

    /// \brief Join the value with the result held
    void join(const value_type& value){ result_ += value;}

    /// \brief Joint the value of the other Sum operation
    void join(const Sum<T>& other);

    /// \brief Query whether the held result is valid
    bool is_result_valid()const{return valid_result_;}

    /// \brief Validate the result
    void validate_result(){valid_result_ = true;}

    /// \brief Invalidate the result
    void invalidate_result(){valid_result_ = false;}

    /// \brief busy wait for the thread that calls it until the
    /// result becomes valid
    result_type get()const;

    /// \brief Attempt to get the result only if it is valid. Perfroms busy
    /// wait is the result is not valid
    result_type get_or_wait()const;

    /// \brief Overload operator!
    bool operator!()const{ return valid_result_;}

private:

    value_type result_;
    bool valid_result_;
};

template<typename T>
void
Sum<T>::join(const Sum<T>& other){

    if(!other){
        return;
    }

    result_ += other.result_;
}

template<typename T>
typename Sum<T>::result_type
Sum<T>::get()const{
    return std::make_pair(result_, valid_result_);
}

template<typename T>
typename Sum<T>::result_type
Sum<T>::get_or_wait()const{

    while(!valid_result_){
        std::this_thread::yield();
    }

    return std::make_pair(result_, valid_result_);
}
}

#endif // REDUCTION_OPERATIONS_H

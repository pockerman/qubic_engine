#ifndef IDENTITY_REDUCTION_H
#define IDENTITY_REDUCTION_H

#include "kernel/parallel/utilities/result_holder.h"

namespace kernel {


template<typename T>
class IdentityReduce: public ResultHolder<T>
{

public:

    //typedef T value_type;
    //typedef std::pair<value_type, bool> result_type;
    using value_type  = typename ResultHolder<T>::value_type;
    using result_type = typename ResultHolder<T>::result_type;
    using ResultHolder<T>::ResultHolder;

    /// \brief Join the given value to the given result
    static void local_join(const value_type& val, value_type& rslt){rslt = val;}

    /// \brief Join the value with the result held
    void join(const value_type& value){ this->get_resource() = value;}

    /// \brief Joint the value of the other Sum operation
    void join(const IdentityReduce<T>& other);

};

template<typename T>
void
IdentityReduce<T>::join(const IdentityReduce<T>& other){

    if(!other.is_result_valid()){
        return;
    }

    this->get_resource() = other.get_resource();
}

}

#endif // IDENTITY_REDUCTION_H

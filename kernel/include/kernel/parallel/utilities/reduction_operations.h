#ifndef REDUCTION_OPERATIONS_H
#define REDUCTION_OPERATIONS_H

#include "kernel/parallel/utilities/result_holder.h"

#include <thread>
#include <utility>

namespace kernel
{

template<typename T>
class Sum: public ResultHolder<T>
{

public:

    using value_type  = typename ResultHolder<T>::value_type;
    using result_type = typename ResultHolder<T>::result_type;
    using ResultHolder<T>::ResultHolder;

    /// \brief Join the given value to the given result
    static void local_join(const value_type& val, value_type& rslt){rslt += val;}

    /// \brief Join the value with the result held
    void join(const value_type& value){ this->get_resource() += value;}

    /// \brief Joint the value of the other Sum operation
    void join(const Sum<T>& other);

};

template<typename T>
void
Sum<T>::join(const Sum<T>& other){

    if(!other.is_result_valid()){
        return;
    }

    this->get_resource() += other.get_resource();
}

}

#endif // REDUCTION_OPERATIONS_H

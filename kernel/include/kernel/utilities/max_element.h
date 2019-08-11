#ifndef MAX_ELEMENT_H
#define MAX_ELEMENT_H

#include <iterator>
#include <iostream>

namespace kernel
{

template<typename ForwardIt>
struct MaxElement
{

    typedef ForwardIt iterator_t;
    typedef typename std::iterator_traits<ForwardIt>::value_type value_t;
    typedef value_t result_t;
    value_t value{};


    void operator()(ForwardIt itr);
    void operator()(const value_t& val);
    value_t get_result()const{return value;}

};


template<typename ForwardIt>
void
MaxElement<ForwardIt>::operator()(ForwardIt itr){

    if(*itr >= value){
        value = *itr;
    }
}

template<typename ForwardIt>
void
MaxElement<ForwardIt>::operator()(const typename MaxElement<ForwardIt>::value_t& val){

    if(val  >= value){
        value = val;
    }
}

}

#endif // MAX_ELEMENT_H

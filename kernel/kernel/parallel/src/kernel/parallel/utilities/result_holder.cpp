#include "kernel/parallel/utilities/result_holder.h"

namespace kernel{

ResultHolder<void>::ResultHolder(bool valid)
    :
   item_(nullptr),
   valid_result_(valid)
{}


void
ResultHolder<void>::get_copy(ResultHolder<void>& other)const{
    other.valid_result_ = valid_result_;
    other.item_ = item_;
}


ResultHolder<void>::result_type
ResultHolder<void>::get()const{
    return std::make_pair(&(const_cast<ResultHolder<void>&>(*this).item_), valid_result_);
}


ResultHolder<void>::result_type
ResultHolder<void>::get_or_wait()const{

    while(!valid_result_){
        std::this_thread::yield();
    }

    return std::make_pair(&(const_cast<ResultHolder<void>&>(*this).item_), valid_result_);
}


ResultHolder<void>::result_type
ResultHolder<void>::get_or_wait_for(uint_t mills)const{

    if(!valid_result_){
        std::this_thread::sleep_for(std::chrono::milliseconds(mills));
    }

    return std::make_pair(&(const_cast<ResultHolder<void>&>(*this).item_), valid_result_);
}

}

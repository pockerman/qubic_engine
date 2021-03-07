#include "kernel/parallel/threading/read_write_lock.h"

namespace kernel
{

RWLock::RWLock()
    :
    num_reads_(0),
    num_writes_(0),
    m_(),
    cvar_()
{}

void RWLock::read_lock(){

    std::unique_lock<std::mutex> lck(m_);

    ///wait until the num_writes becomes zero i.e. no writing is done
    cvar_.wait(lck, [this]{return (this->num_writes_== 0);});
    num_reads_++;

    lck.unlock();
}

void RWLock::read_unlock(){

    std::unique_lock<std::mutex> lck(m_);
    num_reads_--;

    if(num_reads_ == 0)
        cvar_.notify_all();

    lck.unlock();
}

void RWLock::write_lock(){

    std::unique_lock<std::mutex> lck_(m_);

    ///wait until the num_writes becomes zero i.e. no writing is done
    cvar_.wait(lck_, [this]{return (this->num_writes_ >0 || this->num_reads_ > 0);});
    num_writes_++;
}


}

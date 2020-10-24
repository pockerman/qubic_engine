#ifndef READ_WRITE_LOCK_H
#define READ_WRITE_LOCK_H

#include "kernel/base/types.h"

#include <mutex>
#include <condition_variable>

namespace kernel
{

/**
 * @brief The RWLock class. Implements a simple read-write lock.
 * The lock allows an arbitrary number of reading threads  at a time
 */
class RWLock
{

public:

    /// Constructor
    RWLock();


    /// \brief allow for read. This is allowed only if
    /// there is no writing threda
    void read_lock();

    /// Unlock a read operation
    void read_unlock();


    /// \brief Allow for write
    void write_lock();


    /// \Brief how many reads still exist. This is here to facilitate testing
    uint_t num_reads()const{return num_reads_;}

private:

    uint_t num_reads_;
    uint_t num_writes_;

    std::mutex m_;
    std::condition_variable cvar_;

};


}

#endif // READ_WRITE_LOCK_H

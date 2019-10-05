#ifndef ARRAY_PARTITIONER_H
#define ARRAY_PARTITIONER_H

#include "kernel/base/config.h"
#include "kernel/base/types.h"
#include "kernel/utilities/range_1d.h"

#ifdef KERNEL_LOG
#include <chrono>
#include <sstream>
#endif

#ifdef KERNEL_DEBUG
#include <stdexcept> //for std::invalid_argument
#endif

namespace kernel
{

template<typename IteratorTp>
void partition_range(IteratorTp begin, const IteratorTp end,
                     std::vector<range1d<IteratorTp>>& partitions, uint_t n_parts  ){

    uint_t total_work = std::distance(begin, end);

#ifdef KERNEL_DEBUG

    if(n_parts == 0){
        throw std::invalid_argument("Cannot partition range into zero parts");
    }

    if(total_work == 0){
        throw std::invalid_argument("Cannot partition a range with equal start and end points");
    }

#endif

#ifdef KERNEL_LOG
    std::chrono::time_point<std::chrono::system_clock> start_timing = std::chrono::system_clock::now();
#endif

    //clear the partitions
    {
       std::vector<range1d<IteratorTp>> empty;
       partitions.swap(empty);
    }

    partitions.reserve(n_parts);

    if(n_parts == 1){
        partitions.push_back(range1d<IteratorTp>(begin, end));
        return;
    }

    typedef typename range1d<IteratorTp>::value_type value_type;

    //how many elements each partition will get
    uint_t part_load = total_work/n_parts;
    value_type start = begin;

    for(uint_t t=0; t < n_parts-1; ++t){

        value_type finish = start;
        std::advance(finish,part_load);
        partitions.push_back(range1d<IteratorTp>(start, finish));
        start = finish;
   }

    partitions.push_back(range1d<IteratorTp>(start, end));

#ifdef KERNEL_LOG
    std::chrono::time_point<std::chrono::system_clock> end_timing = std::chrono::system_clock::now();
    std::chrono::duration<real_t> dur = end_timing-start_timing;
    std::ostringstream message;
    message<<"SimplePartitioner::partition run time: "<<dur.count();
    //rt_root::log_info(message.str());
#endif

}

/// \brief Partition the range [begin, end)
void partition_range(uint_t begin, uint_t end,
                     std::vector<range1d<uint_t>>& partitions, uint_t n_parts  );




}

#endif // ARRAY_PARTITIONER_H

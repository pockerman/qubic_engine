#include "kernel/parallel/utilities/array_partitioner.h"


namespace kernel
{

void partition_range(uint_t begin, uint_t end,
                     std::vector<range1d<uint_t>>& partitions, uint_t n_parts  ){

    uint_t total_work = end - begin;

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
       std::vector<range1d<uint_t>> empty;
       partitions.swap(empty);
    }

    partitions.reserve(n_parts);

    if(n_parts == 1){
        partitions.push_back(range1d<uint_t>(begin, end));
        return;
    }

    typedef typename range1d<uint_t>::value_type value_type;

    //how many elements each partition will get
    uint_t part_load = total_work/n_parts;
    value_type start = begin;

    for(uint_t t=0; t < n_parts-1; ++t){

        value_type finish = start;
        finish += part_load;
        partitions.push_back(range1d<uint_t>(start, finish));
        start = finish;
   }

    partitions.push_back(range1d<uint_t>(start, end));

#ifdef KERNEL_LOG
    std::chrono::time_point<std::chrono::system_clock> end_timing = std::chrono::system_clock::now();
    std::chrono::duration<real_t> dur = end_timing-start_timing;
    std::ostringstream message;
    message<<"SimplePartitioner::partition run time: "<<dur.count();
#endif



}

}

#include "kernel/utilities/range_1d.h"
#include <stdexcept>
namespace kernel
{

namespace partitioners
{

std::vector<range1d<uint_t>> partition_range_1d(const uint_t begin, const uint_t end, const uint_t nparts){

    uint_t difference = end - begin;

    if(difference == 0){
            throw std::logic_error("Cannot partion a zero length range");
    }

    if(nparts == 0){
        throw std::invalid_argument("Number of partions cannot be zero");
    }


    std::vector<range1d<uint_t>> partitions;
    partitions.reserve(nparts);

/*#ifdef PARFRAMEPP_USE_LOG
    std::chrono::time_point<std::chrono::system_clock> start_timing = std::chrono::system_clock::now();
#endif*/


    if(nparts==1){
        partitions.push_back(range1d<uint_t>(begin,end));
        return partitions;
    }

    //how many elements each partition will get
    uint_t part_load = difference/nparts;
    uint_t start = begin;

    for(uint_t t=0; t<nparts-1; ++t){
     uint_t finish = start;
     finish += part_load;
     partitions.push_back(range1d<uint_t>(start,finish));
     start = finish;
   }

    partitions.push_back(range1d<uint_t>(start,end));

/*
#ifdef PARFRAMEPP_USE_LOG
    std::chrono::time_point<std::chrono::system_clock> end_timing = std::chrono::system_clock::now();
    std::chrono::duration<real_type> dur = end_timing-start_timing;
    std::ostringstream message;
    message<<"SimplePartitioner::partition run time: "<<dur.count();
    rt_root::log_info(message.str());
#endif*/

    return partitions;

}
}//partioners
} //kernel

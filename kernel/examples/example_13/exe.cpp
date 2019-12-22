#include "kernel/base/types.h"
#include "kernel/utilities/range_1d.h"
#include "kernel/parallel/parallel_algos/parallel_for.h"
#include "kernel/parallel/utilities/partitioned_type.h"
#include "kernel/parallel/utilities/result_holder.h"
#include "kernel/parallel/utilities/array_partitioner.h"

#include "kernel/parallel/threading/openmp_executor.h"

#include <random>
#include <iostream>
#include <vector>


int main(){

    using kernel::uint_t;
    using kernel::OMPExecutor;
    using kernel::ResultHolder;
    using kernel::PartitionedType;
    using kernel::range1d;

    // this is the executor use 4 threads
    OMPExecutor pool(4);

    PartitionedType<std::vector<uint_t>> data(100, 0);
    for(uint_t i=0; i<data.size(); ++i){
        data[i] = i;
    }

    std::vector<range1d<uint_t>> partitions;
    kernel::partition_range(0, 100, partitions, pool.get_n_threads());

    data.set_partitions(partitions);

    // add 1 to every item
    auto body = [](uint_t& item){item += 1;};

    // parallel_for blocks until all tasks finish
    ResultHolder<void> result = kernel::parallel_for(data, body, pool);

    if(result.is_result_valid()){

        for(uint_t i=0; i<data.size(); ++i){

            if(data[i] != i+1 ){
                throw std::logic_error("Item not mutated by parallel_for algorithm");
            }
        }

        std::cout<<"Example SUCCESS\n";
    }
    else{
        std::cout<<"Example FAILED\n";
    }

    return 0;
}




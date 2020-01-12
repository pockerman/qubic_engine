#include "cubic_engine/base/cubic_engine_types.h"
#include "cubic_engine/maths/array_stats.h"
#include "kernel/parallel/threading/thread_pool.h"
#include "kernel/parallel/threading/simple_task.h"
#include "kernel/parallel/parallel_algos/parallel_reduce.h"
#include "kernel/parallel/utilities/array_partitioner.h"
#include "kernel/utilities/range_1d.h"
#include "kernel/parallel/utilities/reduction_operations.h"

#include <random>
#include <vector>
#include <chrono>
#include <memory>

namespace exe
{

    using cengine::real_t;
    using cengine::uint_t;
    using cengine::Null;
    using kernel::ThreadPool;
    using kernel::SimpleTaskBase;

struct Task: public SimpleTaskBase<std::vector<real_t>>
{

public:

    // contructor
    Task(real_t mu, real_t std, uint_t  n,
         uint_t begin, uint_t end);

    // get the range the task operates on
    std::pair<uint_t, uint_t> get_range()const;

protected:

    virtual void run()override final;

private:

    real_t mu_;
    real_t std_;
    uint_t sample_size_;
    uint_t start_;
    uint_t end_;

};

Task::Task(real_t mu, real_t std, uint_t  n,
           uint_t begin, uint_t end)
    :
SimpleTaskBase<std::vector<real_t>>(),
mu_(mu),
std_(std),
sample_size_(n),
start_(begin),
end_(end)
{}

void
Task::run(){

    uint_t workload = end_ - start_;
    this->result_.get_resource().resize(workload, 0.0);

    std::random_device rd{};
    std::mt19937 generator{rd()};
    std::normal_distribution<real_t> distribution (mu_, std_);

    std::vector<real_t> sample(sample_size_, 0.0);


    for(uint_t t=0; t<workload; ++t){


        // generate a sample
        for(uint_t s=0; s<sample_size_; ++s){
            sample[s] = distribution(generator);
        }

        auto mean = cengine::sum(sample);
        this->result_.get_resource()[t] = mean/sample_size_;
    }

    // this result is valid
    this->result_.validate_result();
}

std::pair<uint_t, uint_t>
Task::get_range()const{
    return std::pair(start_, end_);
}

}

int main(){
    using namespace exe;

    const uint_t N_THREADS = 4;
    const real_t MU = 0.0;
    const real_t STD = 1.0;
    const uint_t N = 10;
    const uint_t N_ITRS = 1000;

    ThreadPool executor(N_THREADS);

    std::vector<std::unique_ptr<Task>> tasks;
    tasks.reserve(N_THREADS);

    tasks.push_back(std::make_unique<Task>(MU, STD, N, 0, 250));
    tasks.push_back(std::make_unique<Task>(MU, STD, N, 250, 500));
    tasks.push_back(std::make_unique<Task>(MU, STD, N, 500, 750));
    tasks.push_back(std::make_unique<Task>(MU, STD, N, 750, N_ITRS));

    executor.execute(tasks, Null());
    std::vector<real_t> means(N_ITRS, 0.0);

    for(uint_t t=0; t<tasks.size(); ++t){

        if(!tasks[t]->finished() || !tasks[t]->get_result().is_result_valid()){
            throw std::logic_error("Invlaid result detected");
        }

        auto result = tasks[t]->get_result();
        std::vector<real_t>& task_result = result.get_resource();

        auto range =  tasks[t]->get_range();

        uint_t c=0;


        for(uint_t r=range.first; r < range.second; ++r){
            means[r] = task_result[c++];
        }
    }

    typedef std::vector<real_t>::iterator iterator_t;
    std::vector<kernel::range1d<iterator_t>> partitions;
    kernel::partition_range(means.begin(), means.end(), partitions, N_THREADS);
    kernel::Sum<real_t> op;
    kernel::reduce_array(partitions, op, executor);

    auto variance_of_means = cengine::variance(means, op.get_resource()/means.size());
    std::cout<<"Standard error for mean is: "<<std::sqrt(variance_of_means)<<", STD/sqrt(N): "<<STD/std::sqrt(N)<<std::endl;
    return 0
}




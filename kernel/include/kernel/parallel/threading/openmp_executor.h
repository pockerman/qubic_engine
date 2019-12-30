#ifndef OPENMP_EXECUTOR_H
#define OPENMP_EXECUTOR_H

#include "kernel/base/config.h"

#ifdef USE_OPENMP

#include "kernel/base/types.h"

#include <omp.h>
#include <boost/core/noncopyable.hpp>

#include <exception>

namespace kernel {

/**
 * @brief The OMPOptions struct. Options to be passed
 * when executing taks with OMPExecutor
 */
struct OMPOptions
{
    /// \brief An enumeration describing the scheduling for parallel for
    enum class ScheduleType{STATIC, DYNAMIC, GUIDED, AUTO, RUNTIME, DEFAULT};

    ScheduleType schedule;


    OMPOptions(ScheduleType schedule_=ScheduleType::DEFAULT)
        :
          schedule(schedule_)
    {}
};

/**
 * @brief The OMPExecutor class. Executes task using OpenMP threading
 */
class OMPExecutor: private boost::noncopyable
{

public:


    /// \brief Constructor. Set the number of threads
    /// to be used by calling omp_set_num_threads
    OMPExecutor(uint_t n_threads, bool disable_dynamic_teams=false);

    /// \brief Start the thread pool. No effect here simply
    /// to respect contract with ThreadPool
    void start(){}

    /// \brief Terminates the threads in the pool.
    /// For every thread stop() and join() is called.
    /// No effect here simply to respect th contract with ThreadPool
    void close(){}

    /// \brief Returns the number of threads the executor is using
    /// As per the OpenMP standard this returns 1 outside a
    /// parallel region
    uint_t get_n_threads()const;

    /// \brief Returns the number of threads
    uint_t n_processing_elements()const{return get_n_threads();}

    /// \brief Execute the given task list in parallel
    template<typename TaskTypePtr, typename Options>
    void execute(const std::vector<TaskTypePtr>& tasks,  const Options& options=OMPOptions())const;

    /// \brief Execute the given task list in parallel but do not wait
    template<typename TaskTypePtr>
    void parallel_for_nowait(const std::vector<TaskTypePtr>& tasks, OMPOptions::ScheduleType type=OMPOptions::ScheduleType::DEFAULT)const;

    template<typename TaskType, typename OpType>
    void parallel_for_reduce(uint_t iterations, OpType& operation, OMPOptions::ScheduleType type=OMPOptions::ScheduleType::DEFAULT)const;

    /// \brief Has dynamic teams enabled or not
    bool has_dynamic_teams()const{return has_disabled_dyn_teams_;}

private:

    /// \brief Number of threads the executor is using
    uint_t n_threads_;

    /// \brief Whether dynamic teams are active or not
    bool has_disabled_dyn_teams_;
};

inline
OMPExecutor::OMPExecutor(uint_t n_threads, bool disable_dynamic_teams)
    :
   n_threads_(n_threads),
   has_disabled_dyn_teams_(disable_dynamic_teams)

{

   if(disable_dynamic_teams){
       omp_set_dynamic(0);
   }

   // set the number of threads to use. Note
   // that this does not guarantee that the exact number will be used
   // see: https://stackoverflow.com/questions/11095309/openmp-set-num-threads-is-not-working
   omp_set_num_threads(n_threads);
}


template<typename TaskTypePtr, typename Options>
void
OMPExecutor::execute(const std::vector<TaskTypePtr>& tasks, const Options& options)const{

    if(options.schedule == OMPOptions::ScheduleType::DEFAULT){
    #pragma omp parallel for shared(tasks)
        for(uint_t t=0; t<tasks.size(); ++t){
                tasks[t]->execute();
        }
    }
    else{
        throw std::invalid_argument("Invalid scheduling type");
    }
}


template<typename TaskTypePtr>
void
OMPExecutor::parallel_for_nowait(const std::vector<TaskTypePtr>& tasks, OMPOptions::ScheduleType type)const{

    if(type == OMPOptions::ScheduleType::DEFAULT){

        #pragma omp parallel shared(tasks)
            #pragma omp for nowait
                for(uint_t t=0; t<tasks.size(); ++t){
                    tasks[t]->execute();
                }
        }
    else{
        throw std::invalid_argument("Invalid scheduling type");
    }
}


template<typename TaskType, typename OpType>
void
OMPExecutor::parallel_for_reduce(uint_t iterations, OpType& operation, OMPOptions::ScheduleType type)const{

    if(type == OMPOptions::ScheduleType::DEFAULT){

        #pragma omp parallel default(none) shared(operation, iterations)
        {
                    // a task for each thread in the region
                    TaskType task;

                    #pragma omp for nowait
                        for(uint_t t=0; t<iterations; ++t){

                            // execute the task for the number of iterations
                            // tha the thread has been allocated
                            task.execute();

                            // reschedule the task after one execution
                            task.reschedule();
                        }

                     // once the thread is finished join the results
                     #pragma omp critical
                        operation.join(task.get_result());
        }

        operation.validate_result();
    }
    else{
        throw std::invalid_argument("Invalid scheduling type");
    }

}



}

#endif

#endif // OPENMP_EXECUTOR_H

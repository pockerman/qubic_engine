#ifndef OPENMP_EXECUTOR_H
#define OPENMP_EXECUTOR_H

#include "kernel/base/config.h"

#ifdef USE_OPENMP

#include "kernel/base/types.h"
#include <omp.h>

#include <exception>

namespace kernel {


class OMPExecutor
{

public:

    /// \brief An enumeration describing the scheduling for parallel for
    enum class ScheduleType{STATIC, DYNAMIC, GUIDED, AUTO, RUNTIME, DEFAULT};

    /// \brief Constructor. Set the number of threads
    /// to be used by calling omp_set_num_threads
    OMPExecutor(uint_t n_threads, bool disable_dynamic_teams=false);

    /// \brief Returns the number of threads the executor is using
    /// As per the OpenMP standard this returns 1 outside a
    /// parallel region
    uint_t get_n_threads()const;

    /// \brief Returns the number of threads
    uint_t n_processing_elements()const{return get_n_threads();}

    /// \brief Execute the given task list in parallel
    template<typename TaskTypePtr>
    void parallel_for(const std::vector<TaskTypePtr>& tasks, ScheduleType type=ScheduleType::DEFAULT)const;

    /// \brief Execute the given task list in parallel but do not wait
    template<typename TaskTypePtr>
    void parallel_for_nowait(const std::vector<TaskTypePtr>& tasks, ScheduleType type=ScheduleType::DEFAULT)const;

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


template<typename TaskTypePtr>
void
OMPExecutor::parallel_for(const std::vector<TaskTypePtr>& tasks, ScheduleType type)const{

    if(type == ScheduleType::DEFAULT){
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
OMPExecutor::parallel_for_nowait(const std::vector<TaskTypePtr>& tasks, ScheduleType type)const{

    if(type == ScheduleType::DEFAULT){

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



}

#endif

#endif // OPENMP_EXECUTOR_H

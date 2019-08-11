/**
  * Parallel Implementation of Jacobi Iteration
  *
  **/
#include "kernel/executors/thread_pool.h"
#include "kernel/executors/simple_task.h"
#include "kernel/base/algorithm_info.h"
#include "kernel/base/iteration_control.h"
#include "kernel/base/types.h"
#include "kernel/base/kernel_consts.h"
#include "kernel/data_structs/range_1d.h"

#include <blaze/Math.h>

#include <thread>
#include <vector>
#include <iostream>
#include <stdexcept>


namespace  {

    using kernel::uint_t;
    using kernel::real_t;
    using kernel::ThreadPool;
    using kernel::ItrCtrl;
    using kernel::range1d;
    using kernel::SimpleTaskBase;
    using kernel::AlgInfo;

    using Vector = blaze::DynamicVector<real_t>;
    using Matrix = blaze::DynamicMatrix<real_t>;

class JacobiIterator
{

public:

    JacobiIterator(const ItrCtrl& control )
        :
      ctrl_(control)
    {}

    AlgInfo iterate(const Matrix& mat, const Vector& b, Vector& x, ThreadPool& executor);

private:

    /// Inner struct that describes the Jacobi task
    struct JacobiTask: public SimpleTaskBase
    {
        public:

            typedef Vector vector_t;
            typedef Matrix matrix_t;

            JacobiTask(const Vector& b, Vector& old_x, Vector& x,
                       const Matrix& mat, const range1d<uint_t>& range)
                :
                  SimpleTaskBase (),
                  b_(b),
                  old_x_(old_x),
                  x_(x),
                  mat_(mat),
                  range_(range)
            {}

        protected:

            void run()override;

        private:

            const vector_t& b_;
            vector_t& old_x_;
            vector_t& x_;
            const matrix_t& mat_;
            const range1d<uint_t> range_;
    };

    ItrCtrl ctrl_;

    /// A vector of Jacobi tasks to be executed and synchronized
    std::vector<std::unique_ptr<JacobiTask>> tasks_;

    /// checke if the Jacobi tasks finished
    bool jacobi_tasks_finished()const;

    /// reset all tasks to PENDING
    void reset_jacobi_tasks_pending()const;

};

void
JacobiIterator::JacobiTask::run(){

    auto idx_b = range_.begin();
    auto idx_e = range_.end();

    for(; idx_b != idx_e; idx_b++){

        real_t sum = 0.0;

        for(uint_t col=0; col < old_x_.size(); col++ ){

            if( col != idx_b){
                sum += mat_(idx_b, col)*old_x_[col];
            }
        }

        real_t a_ii = mat_(idx_b, idx_b);
        real_t b_i  = b_[idx_b];
        x_[idx_b] = (1./a_ii)*(b_i - sum);
    }
}

bool
JacobiIterator::jacobi_tasks_finished()const{

    for(uint_t t=0; t< tasks_.size(); ++t){

        if(tasks_[t]->get_state() != kernel::TaskBase::TaskState::FINISHED){
            return false;
        }
    }

    return true;
}

void
JacobiIterator::reset_jacobi_tasks_pending()const{

    for(uint_t t=0; t< tasks_.size(); ++t){
        tasks_[t]->set_state(kernel::TaskBase::TaskState::PENDING);
    }
}

AlgInfo
JacobiIterator::iterate(const Matrix& mat, const Vector& b, Vector& x, ThreadPool& executor){

    AlgInfo info;
    info.nthreads = executor.get_n_threads();

    Vector old_solution(x);

    {
        /// partition the space of indices
        std::vector<range1d<uint_t>> ranges = kernel::partitioners::partition_range_1d(static_cast<uint_t>(0),
                                                                                         static_cast<uint_t>(mat.rows()), executor.get_n_threads());

        //create the tasks
        tasks_.reserve(executor.get_n_threads());

        for(uint_t t=0; t< executor.get_n_threads(); ++t){
            tasks_.push_back(std::make_unique<JacobiIterator::JacobiTask>(b, old_solution, x, mat, ranges[t]));
        }
    }

    for(uint_t current_itr=0; current_itr < ctrl_.max_num_itrs; current_itr++){

        /// for each iteration assign the tasks for iteration
        for(uint_t t=0; t<tasks_.size(); ++t){
            executor.add_task(*(tasks_[t].get()));
        }

        /// wait here until the tasks finish
        while(!jacobi_tasks_finished()){
            std::this_thread::yield();
        }

        //reset all the tasks to pending
        reset_jacobi_tasks_pending();

        /// compute the difference between the old and the computed solution
        real_t res = l2Norm(x - old_solution);

        info.residual = res;
        info.niterations = current_itr;

        if( res < ctrl_.tolerance){
            info.converged = true;
            break;
        }

        // set the old solution to the current one
        old_solution = x;
    }

    return info;
}

}

int main(){

    using kernel::ThreadPool;
    using kernel::ItrCtrl;
    using kernel::AlgInfo;

    Vector b(100, 2.0);
    Vector x(100, 0.0);
    Matrix A(100, 100, 0.0);

    /// diagonalize the matrix
    for(uint_t r=0; r < A.rows(); ++r){
        for(uint_t c=0; c < A.columns(); ++c){

            if(r == c){
                A(r,c) = 1.0;
            }
        }
    }

    //create a pool and start it with four threads
    ThreadPool pool(4);

    // create the control
    ItrCtrl control(100, pool.get_n_threads(), kernel::kernel_consts::tolerance());

    // the object responsible for Jacobi iteration
    JacobiIterator iterator(control);

    // this will block until all iterations finish
    auto info = iterator.iterate(A, b, x, pool);

    // print useful information
    std::cout<<info<<std::endl;

    // uncomment this to view the solution
    //std::cout<<x<<std::endl;

    return 0;
}

#ifndef ITERATIVE_ALGORITHM_CONTROLLER_H
#define ITERATIVE_ALGORITHM_CONTROLLER_H

#include "kernel/base/types.h"
#include "kernel/utilities/iterative_algorithm_result.h"
#include <limits>


namespace kernel{

///
/// \brief Controller for iterative algorithms
///
class IterativeAlgorithmController
{
public:

   ///
   /// \brief Constructor
   ///
   IterativeAlgorithmController(uint_t maxIterations, real_t exitTolerance);

   ///
   /// \brief Copy constructor
   ///
   IterativeAlgorithmController(const IterativeAlgorithmController&)=default;

   ///
   /// \brief Move copy constructor
   ///
   IterativeAlgorithmController(IterativeAlgorithmController&&)=default;

   ///
   /// \brief copy assignement
   ///
   IterativeAlgorithmController& operator=(const IterativeAlgorithmController&)=default;

   ///
   /// \brief move copy assignement
   ///
   IterativeAlgorithmController& operator=(IterativeAlgorithmController&&)=default;

   ///
   /// \brief Destructor
   ///
   virtual ~IterativeAlgorithmController()=default;

   ///
   /// \brief Returns true if the iterations of the algorithm should be continued
   ///
   bool continue_iterations();

   ///
   /// \brief show iterations
   ///
   bool show_iterations()const{return show_iterations_;}

	///
   /// \brief show iterations
   ///
   void set_show_iterations_flag(bool flag){show_iterations_ = flag;}
   
   ///
   /// \brief Set the number of threads
   ///
   void set_num_threads(uint_t nthreads){n_threads_ = nthreads;}

   ///
   /// \brief Returns the current iteration index
   ///
   uint_t get_current_iteration()const{
          return current_iteration_idx_;
   }

   ///
   /// \brief Returns the exit tolerance for the algorithm
   ///
   real_t get_exit_tolerance()const{
       return exit_tolerance_;
   }
   
   ///
   /// \brief Return the maximum number of iterations
   ///
   uint_t get_max_iterations()const{return max_iterations_;}
   
   ///
   /// \brief Get the number of threads used
   ///
   uint_t get_num_threads()const{return n_threads_;}

   ///
   /// \brief Returns the state of the controller
   ///
   IterativeAlgorithmResult get_state()const;

   ///
   /// \brief Update the residual
   ///
   void update_residual(real_t res){
     current_res_ = res;
   }

	///
    /// \brief reset
	///
	virtual void reset(const IterativeAlgorithmController& control);

private:


    uint_t max_iterations_;
    real_t exit_tolerance_;
    uint_t current_iteration_idx_;
    real_t current_res_;
    bool show_iterations_;
	uint_t n_threads_;
};

inline
IterativeAlgorithmController::IterativeAlgorithmController(uint_t max_iterations, real_t exit_tolerance)
    :
  max_iterations_(max_iterations),
  exit_tolerance_(exit_tolerance),
  current_iteration_idx_(0),
  current_res_(std::numeric_limits<real_t>::max()),
  show_iterations_(false),
  n_threads_(1)
{}



}

#endif // ITERATIVE_ALGORITHM_CONTROLLER_H

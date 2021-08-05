#ifndef ALGORITHM_BASE_H
#define ALGORITHM_BASE_H

#include "cubic_engine/base/cubic_engine_types.h"
#include "kernel/utilities/iterative_algorithm_controller.h"
#include "kernel/utilities/iterative_algorithm_result.h"


namespace cengine{
namespace rl{
namespace algos {

///
/// \brief The AlgorithmBase class. Base class for deriving
/// RL algorithms
///
class AlgorithmBase
{

public:

    ///
    /// \brief ~AlgorithmBase. Destructor
    ///
    virtual ~AlgorithmBase() = default;

    ///
    /// \brief actions_before_training_iterations. Execute any actions the
    /// algorithm needs before starting the iterations
    ///
    virtual void actions_before_training_iterations() = 0;

    ///
    /// \brief actions_after_training_iterations. Actions to execute after
    /// the training iterations have finisehd
    ///
    virtual void actions_after_training_iterations() = 0;

    ///
    /// \brief step Do one step of the algorithm
    ///
    virtual void step() = 0;

    ///
    /// \brief reset
    ///
    virtual void reset();

    ///
    /// \brief train Iterate to train the agent
    ///
    virtual kernel::IterativeAlgorithmResult train();

    ///
    /// \brief do_minimla_output
    ///
    void do_minimal_output(){itr_ctrl_.set_show_iterations_flag(false);}

    ///
    /// \brief do_verbose_output
    ///
    void do_verbose_output(){itr_ctrl_.set_show_iterations_flag(true);}

    ///
    /// \brief is_verbose
    /// \return
    ///
    bool is_verbose()const{return itr_ctrl_.show_iterations();}

protected:

    ///
    /// \brief AlgorithmBase
    ///
    AlgorithmBase(uint_t n_max_itrs, real_t tolerance);

    ///
    /// \brief itr_ctrl_. The object controlling the iterations
    ///
    kernel::IterativeAlgorithmController itr_ctrl_;


    ///
    /// \brief iter_controller
    /// \return
    ///
    kernel::IterativeAlgorithmController& iter_controller_(){return itr_ctrl_;}
};

}
}
}

#endif // ALGORITHM_BASE_H

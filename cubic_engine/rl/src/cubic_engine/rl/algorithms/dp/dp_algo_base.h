#ifndef DP_ALGO_BASE_H
#define DP_ALGO_BASE_H

#include "cubic_engine/base/cubic_engine_types.h"
#include "cubic_engine/rl/algorithms/algorithm_base.h"
#include "cubic_engine/rl/worlds/discrete_world.h"
#include "kernel/utilities/csv_file_writer.h"

#include <tuple>
#include <vector>
#include <string>

namespace cengine {
namespace rl {
namespace algos {
namespace dp {

///
/// \brief The DPAlgoBase class
///
template<typename TimeStepTp>
class DPAlgoBase: public AlgorithmBase
{
public:

    ///
    /// \brief env_t
    ///
    typedef envs::DiscreteWorldBase<TimeStepTp> env_t;

    ///
    /// \brief value_func_t
    ///
    typedef  DynVec<real_t> value_func_t;

    ///
    ///
    ///
    virtual ~DPAlgoBase() = default;

    ///
    /// \brief actions_before_training_iterations. Execute any actions the
    /// algorithm needs before starting the iterations
    ///
    virtual void actions_before_training_iterations();

    ///
    /// \brief actions_after_training_iterations. Actions to execute after
    /// the training iterations have finisehd
    ///
    virtual void actions_after_training_iterations(){}

    ///
    /// \brief gamma
    ///
    real_t gamma()const{return gamma_;}

    ///
    /// \brief value_func
    ///
    value_func_t& value_func(){return v_;}

    ///
    /// \brief value_func
    ///
    const value_func_t& value_func()const{return v_;}

    ///
    /// \brief reset
    ///
    virtual void reset();

    ///
    /// \brief save. Save the value function
    /// to a CSV file. Applications can override this
    /// behaviour if not suitable.
    /// \param filename
    ///
    virtual void save(const std::string& filename)const;

protected:

    ///
    /// \brief DPAlgoBase
    /// \param name
    ///
    DPAlgoBase(uint_t n_max_itrs, real_t tolerance, real_t gamma, env_t& env);

    ///
    /// \brief env_ref_
    /// \return
    ///
    env_t& env_ref_(){return env_;}

private:

    ///
    /// \brief gamma_
    ///
    real_t gamma_;

    ///
    /// \brief env_
    ///
    env_t& env_;

    ///
    /// \brief v_
    ///
    DynVec<real_t> v_;

};

template<typename TimeStepTp>
DPAlgoBase<TimeStepTp>::DPAlgoBase(uint_t n_max_itrs, real_t tolerance, real_t gamma, env_t& env)
    :
    AlgorithmBase(n_max_itrs, tolerance),
    gamma_(gamma),
    env_(env),
    v_()
{}

template<typename TimeStepTp>
void
DPAlgoBase<TimeStepTp>::reset(){

    env_ref_().reset();
    v_.resize(env_ref_().n_states(), 0.0);

}

template<typename TimeStepTp>
void
DPAlgoBase<TimeStepTp>::actions_before_training_iterations(){
    reset();
}

template<typename TimeStepTp>
void
DPAlgoBase<TimeStepTp>::save(const std::string& filename)const{

    kernel::utilities::CSVWriter writer(filename, ',', true);

    std::vector<std::string> columns(2);
    columns[0] = "State Id";
    columns[1] = "Value";
    writer.write_column_names(columns);

    for(uint_t s=0; s < v_.size(); ++s){
        auto row = std::make_tuple(s, v_[s]);
        writer.write_row(row);
    }
}


}

}

}

}

#endif // DP_ALGO_BASE_H

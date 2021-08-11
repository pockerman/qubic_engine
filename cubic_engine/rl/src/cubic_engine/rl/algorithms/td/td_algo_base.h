#ifndef TD_ALGO_BASE_H
#define TD_ALGO_BASE_H

#include "cubic_engine/rl/algorithms/algorithm_base.h"
#include "cubic_engine/rl/worlds/discrete_world.h"
#include "kernel/utilities/csv_file_writer.h"

#include <unordered_map>
#include <deque>

namespace cengine {
namespace rl {
namespace algos {
namespace td {

///
///
///
template<typename TimeStepTp>
class TDAlgoBase: public AlgorithmBase
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
    /// \brief action_t
    ///
    typedef typename env_t::action_t action_t;

    ///
    /// \brief state_t
    ///
    typedef typename env_t::state_t state_t;

    ///
    /// \brief q_table_t
    ///
    typedef std::unordered_map<action_t, DynVec<real_t>> q_table_t;

    ///
    ///
    ///
    virtual ~TDAlgoBase() = default;

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
    /// \brief eta
    /// \return
    ///
    real_t eta()const{return eta_;}

    ///
    /// \brief value_func
    ///
    //value_func_t& value_func(){return v_;}

    ///
    /// \brief value_func
    ///
    //const value_func_t& value_func()const{return v_;}

    ///
    /// \brief q_table
    /// \return
    ///
    q_table_t& q_table(){return q_;}

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

    ///
    /// \brief plot_frequency
    /// \return
    ///
    uint_t plot_frequency()const{return plot_freq_;}

protected:

     ///
     /// \brief DPAlgoBase
     /// \param name
     ///
     TDAlgoBase(uint_t n_max_itrs, real_t tolerance, real_t gamma, real_t eta, env_t& env);

     ///
     /// \brief env_ref_
     /// \return
     ///
     env_t& env_ref_(){return env_;}


     std::deque<real_t>& tmp_scores(){return tmp_scores_;}
     std::deque<real_t>& avg_scores(){return avg_scores_;}

private:

     ///
     /// \brief plot_freq_
     ///
     uint_t plot_freq_;

     ///
     /// \brief gamma_
     ///
     real_t gamma_;

     ///
     /// \brief alpha_ The learning rate
     ///
     real_t eta_;

     ///
     /// \brief env_
     ///
     env_t& env_;

     ///
     /// \brief v_
     ///
     DynVec<real_t> v_;

     ///
     /// \brief q_
     ///
     std::unordered_map<uint_t, DynVec<real_t>> q_;

     /// monitor the performance

     std::deque<real_t> tmp_scores_;
     std::deque<real_t> avg_scores_;

};

template<typename TimeStepTp>
TDAlgoBase<TimeStepTp>::TDAlgoBase(uint_t n_max_itrs, real_t tolerance, real_t gamma, real_t eta, env_t& env)
    :
    AlgorithmBase(n_max_itrs, tolerance),
    gamma_(gamma),
    eta_(eta),
    env_(env),
    v_()
{}

template<typename TimeStepTp>
void
TDAlgoBase<TimeStepTp>::reset(){

    this->AlgorithmBase::reset();

    env_ref_().reset();
    q_.clear();

}

template<typename TimeStepTp>
void
TDAlgoBase<TimeStepTp>::actions_before_training_iterations(){
    reset();
}

template<typename TimeStepTp>
void
TDAlgoBase<TimeStepTp>::save(const std::string& filename)const{

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

#endif // TD_ALGO_BASE_H

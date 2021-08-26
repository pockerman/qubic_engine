#ifndef TD_ALGO_BASE_H
#define TD_ALGO_BASE_H

#include "cubic_engine/rl/algorithms/algorithm_base.h"
#include "cubic_engine/rl/worlds/discrete_world.h"
#include "kernel/utilities/csv_file_writer.h"

#include <unordered_map>
#include <deque>
#include <vector>
#include <iostream>

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
    /// \brief Destructor
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
    virtual void actions_after_training_iterations();

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
    /// \brief q_table
    /// \return
    ///
    q_table_t& q_table(){return q_;}

    ///
    /// \brief q_table
    ///
    const q_table_t& q_table()const{return q_;}

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

    ///
    /// \brief save_avg_scores
    /// \param filename
    ///
    virtual void save_avg_scores(const std::string& filename)const;

    ///
    /// \brief save_avg_scores
    /// \param filename
    ///
    virtual void save_state_action_function(const std::string& filename)const;

    ///
    /// \brief plot_frequency
    /// \return
    ///
    uint_t plot_frequency()const{return plot_freq_;}

    ///
    /// \brief set_plot_frequency
    ///
    void set_plot_frequency(uint_t f){plot_freq_ = f;}

    ///
    /// \brief make_value_function
    ///
    virtual void make_value_function();

    ///
    /// \brief avg_scores
    ///
    std::deque<real_t>& avg_scores(){return avg_scores_;}

    ///
    /// \brief avg_scores
    ///
    const std::deque<real_t>& avg_scores()const{return avg_scores_;}

protected:

     ///
     /// \brief DPAlgoBase
     /// \param name
     ///
     TDAlgoBase(uint_t n_max_itrs, real_t tolerance, real_t gamma,
                real_t eta, uint_t plot_f, uint_t max_num_iterations_per_episode, env_t& env);

     ///
     /// \brief env_ref_
     /// \return
     ///
     env_t& env_ref_(){return env_;}

     ///
     /// \brief env_ref_
     /// \return
     ///
     const env_t& env_ref_()const{return env_;}

     ///
     /// \brief tmp_scores
     /// \return
     ///
     std::deque<real_t>& tmp_scores(){return tmp_scores_;}

     ///
     /// \brief tmp_scores
     /// \return
     ///
     const std::deque<real_t>& tmp_scores()const{return tmp_scores_;}

     ///
     ///
     ///
     uint_t max_num_iterations_per_episode()const noexcept{return max_num_iterations_per_episode_;}

private:

     ///
     /// \brief plot_freq_
     ///
     uint_t plot_freq_;

     ///
     /// \brief max_num_iterations_per_episode_
     ///
     uint_t max_num_iterations_per_episode_;

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
TDAlgoBase<TimeStepTp>::TDAlgoBase(uint_t n_max_itrs, real_t tolerance, real_t gamma,
                                   real_t eta, uint_t plot_f, uint_t max_num_iterations_per_episode, env_t& env)
    :
    AlgorithmBase(n_max_itrs, tolerance),
    plot_freq_(plot_f),
    max_num_iterations_per_episode_(max_num_iterations_per_episode)
    gamma_(gamma),
    eta_(eta),
    env_(env),
    v_(),
    q_()
{}

template<typename TimeStepTp>
void
TDAlgoBase<TimeStepTp>::reset(){

    this->AlgorithmBase::reset();

    env_ref_().reset();
    q_.clear();

    for(uint_t s=0; s<env_ref_().n_states(); ++s){
        q_.insert_or_assign(s, DynVec<real_t>(env_ref_().n_actions(), 0.0));
    }

    tmp_scores_.clear();
    tmp_scores_.resize(plot_freq_);
    avg_scores_.clear();
    avg_scores_.resize(this->n_max_itrs());
}

template<typename TimeStepTp>
void
TDAlgoBase<TimeStepTp>::actions_before_training_iterations(){
    reset();
}


template<typename TimeStepTp>
void
TDAlgoBase<TimeStepTp>::actions_after_training_iterations(){
    make_value_function();
}


template<typename TimeStepTp>
void TDAlgoBase<TimeStepTp>::make_value_function(){

    // get the number of states
    auto n_states = env_ref_().n_states();
    v_.resize(n_states, 0.0);

    //not all states may have been visited
    auto itr_b = q_.begin();
    auto itr_e = q_.end();

    for(; itr_b != itr_e; ++itr_b){
        v_[itr_b->first] = blaze::max(itr_b->second);
    }
}

template<typename TimeStepTp>
void
TDAlgoBase<TimeStepTp>::save(const std::string& filename)const{

    if(v_.size() == 0){
        return;
    }

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

template<typename TimeStepTp>
void
TDAlgoBase<TimeStepTp>::save_avg_scores(const std::string& filename)const{

    kernel::utilities::CSVWriter writer(filename, ',', true);

    std::vector<std::string> columns(2);
    columns[0] = "Episode Id";
    columns[1] = "Value";
    writer.write_column_names(columns);

    for(uint_t s=0; s < avg_scores().size(); ++s){
        auto row = std::make_tuple(s, avg_scores()[s]);
        writer.write_row(row);
    }
}

template<typename TimeStepTp>
void
TDAlgoBase<TimeStepTp>::save_state_action_function(const std::string& filename)const{

    kernel::utilities::CSVWriter writer(filename, ',', true);

    std::vector<std::string> columns(1 + env_ref_().n_actions());
    columns[0] = "State Id";

    for(uint_t a=0; a<env_ref_().n_actions(); ++a){
        columns[a + 1] = "Action-" + std::to_string(a);
    }
    writer.write_column_names(columns);

    //not all states may have been visited
    auto itr_b = q_.begin();
    auto itr_e = q_.end();

    for(; itr_b != itr_e;  ++itr_b){

        auto vals = itr_b->second;
        auto row = std::make_tuple(itr_b->first, vals[0], vals[1], vals[2], vals[3]);
        writer.write_row(row);
    }
}


}

}

}

}

#endif // TD_ALGO_BASE_H

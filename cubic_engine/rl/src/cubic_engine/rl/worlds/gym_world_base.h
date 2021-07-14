#ifndef GYM_WORLD_BASE_H
#define GYM_WORLD_BASE_H

#include "cubic_engine/rl/worlds/world_base.h"
#include <string>

namespace cengine {
namespace rl {
namespace gym {

///
/// forward declaration
///
class Communicator;
}

namespace worlds {

template<typename StateTp, typename ActionTp>
class GymWorldBase: public WorldBase<StateTp, ActionTp>
{
public:

    ///
    /// \brief state_t
    ///
    typedef typename WorldBase<StateTp, ActionTp>::state_t state_t;

    ///
    /// \brief action_t
    ///
    typedef typename WorldBase<StateTp, ActionTp>::action_t action_t;

    ///
    /// \brief version
    ///
    std::string_view version()const{return v_;}

    ///
    /// \brief update_current_state
    ///
    void update_current_state(const state_t& new_state){current_state_ = new_state;}

    ///
    /// \brief state
    ///
    state_t& state(){return current_state_;}

    ///
    /// \brief render_on_step
    ///
    bool render_on_step()const{return render_on_step_;}

    ///
    /// \brief set_render_step_flag
    ///
    void set_render_step_flag(bool flag){render_on_step_ = flag;}

protected:

    ///
    /// \brief GymWorldBase
    ///
    GymWorldBase(const std::string& name, const std::string& v, gym::Communicator& comm);

    ///
    /// \brief v_. The version of the environment
    ///
    std::string v_;

    ///
    /// \brief comm_ The object responsible for the
    /// communication with Gym OpenAI
    ///
    gym::Communicator* comm_;

    ///
    /// \brief current_state_
    ///
    state_t current_state_;

    ///
    /// \brief render_on_step_
    ///
    bool render_on_step_{false};

    ///
    /// \brief comm_. Access the communicator
    ///
    gym::Communicator* communicator_(){return comm_;}

};

template<typename StateTp, typename ActionTp>
GymWorldBase<StateTp, ActionTp>::GymWorldBase(const std::string& name, const std::string& v, gym::Communicator& comm)
    :
      WorldBase<StateTp, ActionTp> (name),
      v_(v),
      comm_(&comm),
      current_state_(),
      render_on_step_(false)
{}

}

}

}

#endif // GYM_WORLD_BASE_H

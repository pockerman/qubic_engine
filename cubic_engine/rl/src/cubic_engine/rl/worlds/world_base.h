#ifndef WORLD_BASE_H
#define WORLD_BASE_H

#include "cubic_engine/base/cubic_engine_types.h"
#include <boost/noncopyable.hpp>

#include <string>
#include <any>

namespace cengine {
namespace rl {
namespace worlds{

template<typename StateTp, typename ActionTp>
class WorldBase: private boost::noncopyable
{

public:

    ///
    /// \brief state_t
    ///
    typedef StateTp state_t;

    ///
    /// \brief action_t
    ///
    typedef ActionTp action_t;

    ///
    /// \brief Destructor
    ///
    virtual ~WorldBase() = default;

    ///
    /// \brief Transition to a new state by
    /// performing the given action. It returns a tuple
    /// with the following information
    /// arg1: An observation of the environment.
    /// arg2: Amount of reward achieved by the previous action.
    /// arg3: Flag indicating whether it’s time to reset the environment again.
    /// Most (but not all) tasks are divided up into well-defined episodes,
    /// and done being True indicates the episode has terminated.
    /// (For example, perhaps the pole tipped too far, or you lost your last life.)
    /// arg4: The type depends on the subclass overriding this function
    /// diagnostic information useful for debugging. It can sometimes be useful for
    /// learning (for example, it might contain the raw probabilities behind the environment’s last state change).
    /// However, official evaluations of your agent are not allowed to use this for learning.
    ///
    virtual std::tuple<state_t, real_t, bool, std::any> step(const action_t&)=0;

    ///
    /// \brief restart. Restart the world and
    /// return the starting state
    ///
    virtual state_t reset()=0;

    ///
    /// \brief Build the  world
    ///
    virtual  void build(bool reset)=0;

    ///
    /// \brief n_copies Returns the  number of copies of the environment
    ///
    virtual uint_t n_copies()const = 0;

    ///
    /// \brief name
    ///
    std::string name()const{return name_;}

    ///
    /// \brief is_built
    /// \return
    ///
    bool is_built()const{return is_built_;}

    ///
    /// \brief make_is_built
    ///
    void make_is_built(){is_built_ = true;}


protected:

    ///
    /// \brief WorldBase
    /// \param name
    ///
    WorldBase(const std::string& name);

    ///
    /// \brief name_
    ///
    std::string name_;

    ///
    ///
    ///
    bool is_built_{false};

};

template<typename StateTp, typename ActionTp>
WorldBase<StateTp, ActionTp>::WorldBase(const std::string& name)
    :
      name_(name),
      is_built_(false)
{}

}

}

}

#endif // WORLD_BASE_H

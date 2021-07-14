#ifndef A2C_ATARI_LUNAR_LANDER_H
#define A2C_ATARI_LUNAR_LANDER_H

#include "cubic_engine/rl/a2c.h"
#include "cubic_engine/rl/worlds/gym_lunar_lander_world.h"

namespace cengine{
namespace rl{


struct A2CAtariLunarLanderInput: public A2CInput
{
    uint_t max_frames;
    uint_t batch_size;
    uint_t hidden_size{64};
};

class A2CAtariLunarLander: public A2C<worlds::GymLunarLanderWorld>
{
public:

    ///
    /// \brief state_t
    ///
    typedef A2C<worlds::GymLunarLanderWorld>::state_t state_t;

    ///
    /// \brief A2CAtariLunarLander
    ///
    A2CAtariLunarLander(world_t& environment, policies::TorchPolicy& policy, const A2CAtariLunarLanderInput input);


    ///
    /// \brief train
    ///
    virtual void train() override;

private:

    ///
    /// \brief max_frames_
    ///
    uint_t max_frames_;

    ///
    /// \brief batch_size_
    ///
    uint_t batch_size_;

    ///
    /// \brief hidden_size_
    ///
    uint_t hidden_size_;

};

}
}

#endif // A2C_ATARI_LUNAR_LANDER_H

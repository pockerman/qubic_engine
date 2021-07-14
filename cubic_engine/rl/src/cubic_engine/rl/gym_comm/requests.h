#ifndef REQUESTS_H
#define REQUESTS_H

#include "cubic_engine/base/cubic_engine_types.h"

#include <msgpack.hpp>

#include <vector>
#include <string>
#include <any>

namespace cengine{
namespace rl {
namespace  gym {

///
/// \brief 
///
template <class T>
struct Request
{
    Request(const std::string &method, std::shared_ptr<T> param)
        :
          method(method),
          param(param)
    {}

    std::string method;
    std::shared_ptr<T> param;
    MSGPACK_DEFINE_MAP(method, param)
};

///
/// \brief The MakeParam struct
///
struct MakeRequest
{
    std::string env_name;
    int num_envs;
    MSGPACK_DEFINE_MAP(env_name, num_envs);
};


///
/// \brief The MakeResponse struct. The response returned
/// by the server when a MakeRequest is issued
///
struct MakeResponse
{
    std::string result;
    MSGPACK_DEFINE_MAP(result);
};

///
/// \brief The InfoParam struct
///
struct InfoParam
{
    int x;
    MSGPACK_DEFINE_MAP(x);
};


struct ResetRequest
{
    int x;
    MSGPACK_DEFINE_MAP(x);
};

struct StepRequest
{
    int action;
    bool render;
    MSGPACK_DEFINE_MAP(action, render);
};

struct GetDynamicsParam
{
    uint_t state;
    uint_t action;

    MSGPACK_DEFINE_MAP(state, action);
};


struct DiscreteStepResponse
{
    uint_t observation;
    real_t reward;
    bool done;
    MSGPACK_DEFINE_MAP(observation, reward, done);
};

struct InfoResponse
{
    std::string action_space_type;
    std::vector<int64_t> action_space_shape;
    std::string observation_space_type;
    std::vector<int64_t> observation_space_shape;
    uint_t observation_space_size;
    MSGPACK_DEFINE_MAP(action_space_type, action_space_shape,
                       observation_space_type, observation_space_shape,
                       observation_space_size);
};

struct DiscreteDynamicsResponse
{
    std::vector<real_t> probability;
    std::vector<uint_t> next_state;
    std::vector<real_t> reward;
    std::vector<bool> done;
    MSGPACK_DEFINE_MAP(probability, next_state, reward, done);
};

struct DiscreteResetResponse
{
    uint_t x;
    MSGPACK_DEFINE_MAP(x);
};

struct ResetResponse
{
    std::vector<std::vector<float>> observation;
    MSGPACK_DEFINE_MAP(observation);
};


struct CnnResetResponse
{
    std::vector<std::vector<std::vector<std::vector<float>>>> observation;
    MSGPACK_DEFINE_MAP(observation);
};

struct MlpResetResponse
{
    std::vector<std::vector<float>> observation;
    MSGPACK_DEFINE_MAP(observation);
};

struct StepResponse
{
    std::vector<std::vector<float>> reward;
    std::vector<std::vector<bool>> done;
    std::vector<std::vector<float>> real_reward;
};

struct CnnStepResponse : StepResponse
{
    std::vector<std::vector<std::vector<std::vector<float>>>> observation;
    MSGPACK_DEFINE_MAP(observation, reward, done, real_reward);
};

struct MlpStepResponse : StepResponse
{
    std::vector<std::vector<float>> observation;
    MSGPACK_DEFINE_MAP(observation, reward, done, real_reward);
};


}
}
}

#endif

#include "cubic_engine/rl/policies/uniform_discrete_policy.h"
#include <gtest/gtest.h>

namespace{

using cengine::rl::policies::UniformDiscretePolicy;


}


TEST(TestUniformDiscretePolicy, Constructor) {

    try{
        UniformDiscretePolicy policy(4, 4);
    }
    catch(...){
        FAIL()<<"Constructing UniformDiscretePolicy failed";
    }
}

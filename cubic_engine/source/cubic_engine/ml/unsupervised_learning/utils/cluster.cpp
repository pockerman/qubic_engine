#include "cubic_engine/ml/unsupervised_learning/utils/cluster.h"

namespace cengine
{

real_t calculate_cluster_purity(const std::map<uint_t, uint_t>& classes, uint_t sum){

    auto pij = 0.0;

    auto itr_begin = classes.begin();
    auto itr_end = classes.end();

    for(; itr_begin != itr_end; ++itr_begin){

        auto prob = static_cast<real_t>(itr_begin->second)/static_cast<real_t>(sum);

        if( prob > pij){
            pij = prob;
        }
    }

    return pij;
}
}

#include "parml/unsupervised_learning/kmeans_base.h"

namespace parml
{
    
    std::ostream& 
    KMeansControl::print(std::ostream& out)const{ 
        
        this->AlgControl::print(out);
        out<<"Number of clusters: "<<k<<std::endl;
        out<<"Random restart: "<<(random_restart_on_empty_cluster?"Yes":"No")<<std::endl;
        out<<"Continue with empty clusters: "<<(continue_on_empty_cluster?"Yes":"No")<<std::endl;
        out<<"Number of features: "<<n_features<<std::endl;
        out<<"Number of n_training_examples: "<<n_training_examples<<std::endl;
        return out;
    }
    
}

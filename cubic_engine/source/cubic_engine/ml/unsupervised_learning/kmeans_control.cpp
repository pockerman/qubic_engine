#include "parml/unsupervised_learning/kmeans_control.h"

namespace parml
{
    
std::ostream& 
KMeansControl::print(std::ostream& out)const{ 
        
        this->AlgControl::print(out);
        out<<"# clusters: "<<k<<std::endl;
        out<<"# features: "<<n_features<<std::endl;
        out<<"# training examples: "<<n_training_examples<<std::endl;
        out<<"Random restart: "<<(random_restart_on_empty_cluster?"Yes":"No")<<std::endl;
        out<<"Continue with empty clusters: "<<(continue_on_empty_cluster?"Yes":"No")<<std::endl;
        return out;
    }   
    
}

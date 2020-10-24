#include "parml/algorithms/gradient_descent_info_control.h"

namespace parml
{
    
    std::ostream& 
    GDControl::print(std::ostream& out)const{
        
        out<<"# examples: "<<n_training_examples<<std::endl;
        out<<"# features: "<<n_features<<std::endl;
        this->AlgControl::print(out);
        out<<"Learning rate: "<<learning_rate<<std::endl;
        
        return out;   
    }
    
    std::ostream& 
    GDInfo::print(std::ostream& out)const{
        out<<"GD Type: "<<gradient_descent_type::type_to_string(type)<<std::endl;
        this->AlgInfo::print(out);
        out<<"Learning rate: "<<learning_rate<<std::endl;
        return out;  
    }
}

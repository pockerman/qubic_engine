#include "parml/objective_functions/squared_loss_func.h"
#include "parml/functions/vector_metric_function_base.h"
#include "parml/datasets/armadillo_vec_dataset.h"


namespace parml
{
  
template<typename T>
real_type
SquaredLossFunc<ArmaVecDataSet<T> >::evaluate(const ArmaVecDataSet<T>& target, 
                                              const ArmaVecDataSet<T>& prediction) const{
    
    real_type diff = metric_->evaluate(target,prediction);
    
    if(metric_->type()==distance_type::Type::L2NORM){
        
        return diff;
    }
    
    return diff*diff;
}  

//explicit instantiations
template class SquaredLossFunc<ArmaVecDataSet<real_type> >;
template class SquaredLossFunc<ArmaVecDataSet<size_type> >;
    
    
}

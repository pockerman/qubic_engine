#include "parml/instance_learning/knn_cput_c11.h"

namespace parml
{
    
   Knn<knn_type::Type::CPUT_C11_KNN>::Knn()
           :
    parframepp::par_indexed_object(),
    parframepp::model_impl_base()
   {}
}

#include "parml/algorithms/cput_c11_batch_gradient_descent.h"

namespace parml
{
    
    Gd<gradient_descent_type::Type::CPUT_C11_BGD>::Gd(const GDControl& data)
            :
    parframepp::par_indexed_object(),
    data_(data)
    {}
}

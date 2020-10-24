#include "parml/algorithms/serial_batch_gradient_descent.h"

namespace parml
{
    
    Gd<gradient_descent_type::Type::SERIAL_BGD/*serial,batch_gd*/>::Gd(const GDControl& data)
            :
    data_(data)
    {}    
}

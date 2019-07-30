#include "parml/algorithms/gradient_descent_type.h"

namespace parml
{
    
    std::string 
    gradient_descent_type::type_to_string(gradient_descent_type::Type t){
        
        switch(t){
            
            case gradient_descent_type::Type::SERIAL_BGD:
                return "SERIAL_BGD";
            case gradient_descent_type::Type::CPUT_C11_BGD:
                return "CPUT_C11_BGD";
            case gradient_descent_type::Type::MPI_BGD:
                return "MPI_BGD";
            case gradient_descent_type::Type::MPI_SGD:
                return "MPI_SGD";
            case gradient_descent_type::Type::SERIAL_SGD:
                return "SERIAL_SGD";
            case gradient_descent_type::Type::CPUT_C11_SGD:
                return "CPUT_C11_SGD";
        }
        
        
        return "INVALID_TYPE";
    }
}

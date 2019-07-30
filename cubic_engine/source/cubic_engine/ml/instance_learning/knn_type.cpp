#include "parml/instance_learning/knn_type.h"

namespace parml
{
    
   std::string 
   knn_type::type_to_string(knn_type::Type t){
       
       
       switch(t)
       {
           
           case knn_type::Type::SERIAL_KNN:
               return "SERIAL_KNN";
           case knn_type::Type::CPUT_C11_KNN:
               return "CPUT_C11_KNN";
           case knn_type::Type::MPI_KNN:
               return "MPI_KNN";
       }
       
       
       return "INVALID_TYPE";
   } 
    
    
}

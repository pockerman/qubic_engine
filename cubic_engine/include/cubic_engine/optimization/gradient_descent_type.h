/* 
 * File:   gradient_descent_type.h
 * Author: david
 *
 * Created on September 30, 2016, 6:22 PM
 */

#ifndef GRADIENT_DESCENT_TYPE_H
#define	GRADIENT_DESCENT_TYPE_H

#include <string>

namespace parml
{
    
    
    
    struct gradient_descent_type
    {
        
        enum class Type{SERIAL_BGD,CPUT_C11_BGD,MPI_BGD,
                        SERIAL_SGD, CPUT_C11_SGD, MPI_SGD,INVALID_TYPE};
        
        /**
         * @brief Return a string representation of the given type
         */
        static std::string type_to_string(Type t);
    };
    
    //Forward declarations
    template<gradient_descent_type::Type t/*typename RM,typename GDType*/> class Gd;
    
    /**
      * @brief Placeholder for batch gradient descent
    */
    //struct batch_gd{};    
}



#endif	/* GRADIENT_DESCENT_TYPE_H */


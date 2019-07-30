/* 
 * File:   knn_type.h
 * Author: david
 *
 * Created on October 17, 2016, 8:52 PM
 */

#ifndef KNN_TYPE_H
#define	KNN_TYPE_H

#include <string>

namespace parml
{
    
  /**
   * @brief struct that holds the types
   * of knn algorithm the library supports
   */
    struct knn_type
    {
        /**
         * @brief enum of the types of Knn algorithm the library supports
         */
        enum class Type{SERIAL_KNN,CPUT_C11_KNN,MPI_KNN,INVALID_TYPE};
        
        /**
         * @brief Convert to string the given Type
         */
        static std::string type_to_string(Type t);
        
    };
    
    //forward declarations
    template<knn_type::Type t> class Knn;
    
    
    
}



#endif	/* KNN_TYPE_H */


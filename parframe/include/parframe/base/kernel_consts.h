#ifndef KERNEL_CONSTS_H
#define KERNEL_CONSTS_H

#include "parframe/base/types.h"
#include <boost/noncopyable.hpp>

namespace parframe
{

	/**
	 * @brief Class that handles common quantities used around
	 * the library.
	 */
class kernel_consts: private boost::noncopyable
{

public:

     /**
        *@brief Returns the value of the tolerance constant. Default is static_cast<real_type>(1.0e-8)
     */
     static real_t tolerance(){return tol_;}

     /**
      * @brief Dummy unit constant indicating the absence of a metric unit
      */
     static std::string dummy_unit(){return "DUMMY_UNIT";}

     /**
      * @brief Dummy string
      */
     static std::string dummy_string(){return "DUMMY_STR";}

     /**
       * @brief Returns the value of the invalid size_type constant. Default is static_cast<size_type>(-1)
       */
     static uint_t invalid_size_type(){return static_cast<uint_t>(-1);}

     /**
      *@brief Initialize the default constants
      */
     static void initialize();

private:

      /**
       * @brief Constructor
       */
      kernel_consts();


        /**
            * @brief The tolerance constant
            */
        static real_t tol_;

};



}
#endif

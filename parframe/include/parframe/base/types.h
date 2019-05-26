#ifndef TYPES_H
#define TYPES_H

#include <cstddef>
#include <string>

namespace parframe
{

	/// \brief Expose various shortcuts 
	typedef std::size_t uint_t; 
	
	/// \brief Configure the double precision type
	#ifdef KERNEL_REAL_TYPE_FLOAT
        typedef float real_t;
	#else
        typedef double real_t;
	#endif
}

namespace kernel
{
    /// \brief Expose various shortcuts
    typedef std::size_t uint_t;

    /// \brief Configure the double precision type
    #ifdef KERNEL_REAL_TYPE_FLOAT
        typedef float real_t;
    #else
        typedef double real_t;
    #endif

}

    //typedef parframe::uint_t uint_t;
    //typedef parframe::real_type real_type;

#endif

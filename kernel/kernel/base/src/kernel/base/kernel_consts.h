#ifndef KERNEL_CONSTS_H
#define KERNEL_CONSTS_H

#include "kernel/base/types.h"
#include "kernel/base/version.h"
#include <boost/noncopyable.hpp>

#include <string>

namespace kernel{

///
/// \brief The KernelConsts class. Common quantities used around  the library.
///
class KernelConsts: private boost::noncopyable
{

public:

    static constexpr uint_t INVALID_SIZE_TYPE = static_cast<uint_t>(-1);

    ///
    /// \brief Returns a string describing the library version
    ///
    static std::string kernel_version(){return std::string(KERNELLIB_VERSION);}

    ///
    /// \brief Returns the value of the tolerance constant.
    /// Default is static_cast<real_type>(1.0e-8)
    ///
    static real_t tolerance(){return tol_;}

    ///
    /// \brief Dummy unit constant indicating the absence of a metric unit
    ///
    static std::string dummy_unit(){return "DUMMY_UNIT";}

    ///
    /// \brief Returns "EOF"
    ///
    static std::string eof_string(){return "EOF";}

    ///
    /// \brief Returns the INFO string
    ///
    static std::string info_str(){return "INFO: ";}

    ///
    /// \brief warning_str Returns the WARNING string
    ///
    static std::string warning_str(){return "WARNING: ";}

    ///
    /// \brief Dummy string
    ///
    static std::string dummy_string(){return "DUMMY_STR";}

    ///
    /// \brief Returns static_cast<uint_t>(-1)
    ///
    static uint_t invalid_size_type(){return static_cast<uint_t>(-1);}

    ///
    /// \brief Initialize the default constants
    ///
    static void initialize();
	
    ///
    /// \brief Constructor
    ///
    KernelConsts()=delete;

private:

    ///
    /// \brief The tolerance constant
    ///
    static real_t tol_;
};



}
#endif

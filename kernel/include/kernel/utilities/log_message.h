#ifndef LOG_MESSAGE_H
#define LOG_MESSAGE_H

#include "kernel/base/config.h"

#ifdef USE_LOG

#include <boost/core/noncopyable.hpp>
#include <string>

namespace kernel
{

struct LogMsg: private boost::noncopyable
{

    /// \brief Enumeration of the types of messages
    enum class Type{ERROR,WARNING,INFO,INVALID_TYPE};

    /// \brief  Return an std::string representation of the
    /// of the LogMsg::Type
    static std::string type_to_string(Type t);

    /// \brief Return a rich std::string representation of the
    /// form [LogMsg::type_to_string(Type t)] of the LogMsg::Type
    static std::string type_to_rich_string(Type t);
private:

   /// \brief Constructor. Private since this struct only provides
   /// a static interface.
   LogMsg()
   {}

};



}

#endif
#endif // LOG_MESSAGE_H

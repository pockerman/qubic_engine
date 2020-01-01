#ifndef LOG_SECTION_H
#define LOG_SECTION_H

#include "kernel/base/config.h"

#ifdef USE_LOG

#include "kernel/utilities/log_message.h"
#include <sstream> // for std::stringstream

namespace kernel
{

/// \brief The LogSection class. This class acts as
/// a wrapper around the functionality of the Logger class and provides the atomicity of the
/// logging operation and the formatting of the log output.
class LogSection
{

public:

    /// \brief Constructor. Create a LogSection by passing the
    //type of the message to be written in the log file
    LogSection (LogMsg::Type t ) ;

    /// \brief  Destructor calls commit()
    ~LogSection () ;

    /// \brief Write the message into the log file. The parametric type Type
    /// must be streamable meaning overloading operator<<. This function
    /// actually accumulates the message given into the internal buffer.
    /// In order to be written into the log file the commit function must be called.
    template <typename Type>
    void log( const Type & message ) ;

    /// \brief Commit, i.e. write, to the log file the accummulated internal buffer
    void commit () ;

private:

    ///\brief The type of the message to log
    LogMsg::Type t_;

    /// \brief Intermediate buffer for log messages
    std::stringstream message_ ;

    /// \brief Flag indicating whether the message has been commited
    /// default is false. If commit() is called by the specific
    /// instance then this flag becomes true
    bool commited_;

};


//template and inline member functions

inline
LogSection::LogSection(LogMsg::Type t )
        :
        t_(t),
        message_(""),
        commited_(false)
{}


template <typename Type>
inline
void
LogSection::log( const Type & message ){

   message_ << message;
}


template<typename Type>
inline
LogSection& operator<<(LogSection & logsection , const Type& message )
{
   logsection.log (message) ;
   return logsection ;
}


}

#endif
#endif // LOG_SECTION_H

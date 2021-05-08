#ifndef LOGGER_H
#define LOGGER_H

#include "kernel/base/config.h"

#ifdef USE_LOG

#include "kernel/utilities/log_message.h"
#include "kernel/utilities/log_section.h"

#include <mutex>
#include <fstream>
#include <sstream>
#include <string>
#include <chrono>
#include <ctime>

//boost
#include <boost/core/noncopyable.hpp>

namespace kernel
{

///
/// \brief Singleton class to be used for logging.
///
class Logger: private boost::noncopyable
{

public:

    ///
    /// \brief Log the given message
    ///
    template <typename Type>
    void log(const Type& message );

    ///
    /// \brief Set the file name used for logging
    ///
    static void set_log_file_name(const std::string& filename);

    ///
    /// \brief Log a warning  message
    ///
    template<typename Type>
    static void log_warning(const Type& message );

    ///
    /// \brief Log an Error  message
    ///
    template<typename Type>
    static void log_error(const Type& message );

    ///
    /// \brief Log an Info  message
    ///
    template<typename Type>
    static void log_info(const Type& message );

private:

   //private ctor so that client code
   //cannot explicitly instantiate objects
   //of this class
   Logger()
   :
   mutex_(),
   log_(),
   has_been_opened_(false),
   file_name_("log_file.log")
  {}

  /// \brief Access to the logger instance
  static Logger& instance();

  /// \brief Destructor
  ~Logger();

  /// \brief the mutex to lock
  std::mutex mutex_;

  /// \brief The log file
  std::ofstream log_;

  //flag indicating that the file has date info
  bool has_been_opened_;

  ///
  /// \brief The log file name. The default is "log_file.log"
  ///
  std::string file_name_;

  /// \brief Log the start/end time points
  void log_start_end(const std::string& start_end);

  friend class LogSection;

};

template<typename Type>
void
Logger::log(const Type& message ){


  std::lock_guard<std::mutex> lock(mutex_);

  std::ostringstream filename;
  filename<<file_name_;

  //open the file
  log_.open(filename.str().c_str(),std::ios_base::app);

   if(log_.is_open()){

      //check if the file has date info
      if(has_been_opened_ == false){

              const static std::string msg("--LOG BEGIN: ");
              log_start_end(msg);
              has_been_opened_ = true;
      }

      log_ << message<<std::endl;
      log_.flush();
      log_.close();
   }
}



template<typename Type>
void
Logger::log_warning(const Type& message ){

    LogSection section(LogMsg::Type::WARNING);
    section.log(message);
    section.commit();
}


template<typename Type>
void
Logger::log_error(const Type& message ){

    LogSection section(LogMsg::Type::ERROR);
    section.log(message);
    section.commit();
}


template<typename Type>
void
Logger::log_info(const Type& message ){

    LogSection section(LogMsg::Type::INFO);
    section.log(message);
    section.commit();
}

}

#endif
#endif // LOGGER_H

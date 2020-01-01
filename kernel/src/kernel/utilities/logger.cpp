#include "kernel/base/config.h"

#ifdef USE_LOG

#include "kernel/utilities/logger.h"

namespace kernel
{

Logger&
Logger::instance(){

    static Logger logger;
    return logger;
}


void
Logger::set_log_file_name(const std::string& filename){

  instance().file_name_ = filename;
}

Logger::~Logger(){

     std::ostringstream filename;
     filename<<file_name_;
     //when the logger goes out of scope write into
     //the file
     //open the file
     log_.open(filename.str().c_str(),std::ios_base::app);

     if(log_.is_open()){

        const static std::string msg("--LOG END: ");
        log_start_end(msg);
        log_.close();
     }
}

void
Logger::log_start_end(const std::string& start_end){


  if(log_.is_open()){

    //get system date and time
    std::chrono::time_point<std::chrono::system_clock> now;
    now = std::chrono::system_clock::now();
    std::time_t now_time = std::chrono::system_clock::to_time_t(now);

    log_<<start_end+" "<<std::ctime(&now_time)<<"----------------------------------------------------"<<std::endl;
    log_.flush();
  }
}


}

#endif

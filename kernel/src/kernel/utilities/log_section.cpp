#include "kernel/base/config.h"

#ifdef USE_LOG

#include "kernel/utilities/log_section.h"
#include "kernel/utilities/logger.h"

namespace kernel
{

LogSection::~LogSection (){

    commit();
}

void
LogSection::commit(){

   if(commited_ == false){

        //get an instance of the logger
        Logger& logger = Logger::instance();

        //format the message
        std::ostringstream oss;

        //const std::string type(LogMsg::type_to_rich_string(t_));
        //oss<<type.c_str();
        oss<<LogMsg::type_to_rich_string(t_)<<" "<<message_.str();

        logger.log(oss.str());

        // Resetting the message buffer
        message_.str (" ") ;
        message_.clear() ;
        commited_ = true;
   }

}

}
#endif

#include "parframe/utilities/file_formats.h"

namespace kernel
{

std::string
FileFormats::type_to_string(FileFormats::Type t){

   switch(t)
   {
      case FileFormats::Type::CSV:
         return "csv";
   }

   return "INVALID_TYPE";
}

}

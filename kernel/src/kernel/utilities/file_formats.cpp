#include "kernel/utilities/file_formats.h"

namespace kernel
{

std::string
FileFormats::type_to_string(FileFormats::Type t){

   switch(t)
   {
      case FileFormats::Type::CSV:
         return "csv";
     case FileFormats::Type::VTK:
        return "vtk";
   }

   return "INVALID_TYPE";
}

}

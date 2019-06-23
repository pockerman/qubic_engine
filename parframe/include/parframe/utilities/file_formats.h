#ifndef FILE_FORMATS_H
#define FILE_FORMATS_H

#include <string>

namespace kernel
{

/**
  *@brief Small struct that holds an enumeration for various
  *file formats
*/
struct FileFormats
{

  /**
    *@brief File formats types
    */
  enum class Type{CSV=0, INVALID_TYPE};

  /**
    *@brief Return an std::string representation of the given file format type
    */
  static std::string type_to_string(Type t);
};

}

#endif // FILE_FORMATS_H

#ifndef PRECONDITIONER_TYPE_H
#define PRECONDITIONER_TYPE_H

#include <string>

namespace kernel{
namespace numerics{


  /// \brief  useful enumeration of the preconditioner types we support
  enum class PreconditionerType{JACOBI, ILU,LU,ICC,INVALID_PREC};

  /// \brief Return a string with the preconditioner name
  std::string preconditioner_to_string(PreconditionerType type);
}
}

#endif // PRECONDITIONER_TYPE_H

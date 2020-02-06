#ifndef PRECONDITIONER_TYPE_H
#define PRECONDITIONER_TYPE_H

namespace kernel{
namespace numerics{


  /// \brief  useful enumeration of the preconditioner types we support
  enum class PreconditionerType{JACOBI, ILU,LU,ICC,INVALID_PREC};
}
}

#endif // PRECONDITIONER_TYPE_H

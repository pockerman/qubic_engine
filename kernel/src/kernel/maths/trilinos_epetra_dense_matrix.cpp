#include "kernel/maths/trilinos_epetra_dense_matrix.h"

#ifdef USE_TRILINOS

namespace kernel {
namespace maths {
namespace algebra {

TrilinosEpetraDenseMatrix::TrilinosEpetraDenseMatrix()
    :
      mat_()
{}


TrilinosEpetraDenseMatrix::TrilinosEpetraDenseMatrix(uint_t m, uint_t n)
    :
      mat_(static_cast<trilinos_int_t>(m), static_cast<trilinos_int_t>(n))
{}


}

}

}

#endif

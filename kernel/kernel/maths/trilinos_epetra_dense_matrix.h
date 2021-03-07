#ifndef TRILINOS_EPETRA_DENSE_MATRIX_H
#define TRILINOS_EPETRA_DENSE_MATRIX_H

#include "kernel/base/config.h"

#ifdef USE_TRILINOS

#include "kernel/base/types.h"

#ifdef USE_TRILINOS_LONG_LONG_TYPE
#include <Epetra_LongLongSerialDenseMatrix.h>
#else
#include <Epetra_IntSerialDenseMatrix.h>
#endif

namespace kernel {
namespace maths {
namespace algebra{

///
/// \brief The TrilinosEpetraDenseMatrix class. Wrapper
/// to the Epetra_SerialDenseMatrix
///
class TrilinosEpetraDenseMatrix
{
public:

#ifdef USE_TRILINOS_LONG_LONG_TYPE
    typedef Epetra_LongLongSerialDenseMatrix trilinos_matrix_t;
#else
    typedef Epetra_IntSerialDenseMatrix trilinos_matrix_t;
#endif

    typedef real_t value_t;
    typedef std::vector<real_t> row_entries_t;
    typedef std::vector<trilinos_int_t> row_indices_t;

    ///
    /// \brief TrilinosEpetraDenseMatrix. Constructor
    ///
    TrilinosEpetraDenseMatrix();

    ///
    /// \brief TrilinosEpetraDenseMatrix Constructor
    /// Create a mxn matrix
    ///
    TrilinosEpetraDenseMatrix(uint_t m, uint_t n);


private:

    ///
    /// \brief mat_ The trilinos matrix
    ///
    trilinos_matrix_t mat_;
};


}

}

}

#endif
#endif // TRILINOS_EPETRA_DENSE_MATRIX_H

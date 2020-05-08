#ifndef TYPES_H
#define TYPES_H

#include "kernel/base/config.h"

#include <blaze/Math.h>
#include <cstddef>
#include <string>


namespace kernel
{
    /// \brief unsigned int type
    typedef std::size_t uint_t;

    /// \brief Configure the double precision type
    #ifdef KERNEL_REAL_TYPE_FLOAT
        typedef float real_t;
    #else
        typedef double real_t;
    #endif

    /// \brief General matrix type
    template<typename T>
    using DynMat = blaze::DynamicMatrix<T, blaze::rowMajor>;

    /// \brief General diagonal matrix
    template<typename T>
    using DiagMat = blaze::DiagonalMatrix<DynMat<T>>;

    /// \brief Identity matrix
    template<typename T>
    using IdentityMatrix = blaze::IdentityMatrix<T>;

    /// \brief General Sparse matrix
    template<typename T>
    using SparseMatrix = blaze::CompressedMatrix<T, blaze::rowMajor>;

    /// \brief General vector type. By default this is
    /// a column vector
    template<typename T>
    using DynVec = blaze::DynamicVector<T, blaze::columnVector>;

    /// \brief Null type. Simple placeholder
    struct Null{};

}

#endif

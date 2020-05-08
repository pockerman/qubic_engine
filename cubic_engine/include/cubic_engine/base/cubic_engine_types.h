#ifndef CUBIC_ENGINE_TYPES_H
#define CUBIC_ENGINE_TYPES_H

#include <cstddef>
#include "kernel/base/types.h"
#include <blaze/Math.h>


namespace cengine
{

    /// \brief General matrix type
    template<typename T>
    using DynMat = blaze::DynamicMatrix<T>;

    /// \brief General diagonal matrix
    template<typename T>
    using DiagMat = blaze::DiagonalMatrix<DynMat<T>>;

    /// \brief Identity matrix
    template<typename T>
    using IdentityMatrix = blaze::IdentityMatrix<T>;

    /// \brief General Sparse matrix
    template<typename T>
    using SparseMatrix = blaze::CompressedMatrix<T, blaze::rowMajor>;

    /// \brief General vector type
    template<typename T>
    using DynVec = blaze::DynamicVector<T>;

    /// \brief Basic primitive data types
    using real_t = kernel::real_t;
    using uint_t = kernel::uint_t;

    /// \brief Null type
    using Null = kernel::Null;

}

#endif // CUBIC_ENGINE_TYPES_H

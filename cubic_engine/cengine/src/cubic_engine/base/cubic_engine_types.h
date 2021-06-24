#ifndef CUBIC_ENGINE_TYPES_H
#define CUBIC_ENGINE_TYPES_H

#include "kernel/base/config.h"
#include "kernel/base/types.h"
#include <blaze/Math.h>
#include <cstddef>

namespace cengine{

    ///
    /// \brief General matrix type
    ///
    template<typename T>
    using DynMat = blaze::DynamicMatrix<T>;

    ///
    /// \brief General diagonal matrix
    ///
    template<typename T>
    using DiagMat = blaze::DiagonalMatrix<DynMat<T>>;

    ///
    /// \brief Identity matrix
    ///
    template<typename T>
    using IdentityMatrix = blaze::IdentityMatrix<T>;

    ///
    /// \brief General Sparse matrix
    ///
    template<typename T>
    using SparseMatrix = blaze::CompressedMatrix<T, blaze::rowMajor>;

    ///
    /// \brief General vector type
    ///
    template<typename T>
    using DynVec = blaze::DynamicVector<T>;

    ///
    /// \brief General real type
    ///
    using real_t = kernel::real_t;

    ///
    /// \brief General unsigned int type
    ///
    using uint_t = kernel::uint_t;

    ///
    /// \brief Null type
    ///
    using Null = kernel::Null;
    
#ifdef USE_PYTORCH
    using torch_t = kernel::torch_t;
    using torch_int_t = kernel::torch_int_t;
#endif
    
    

}

#endif // CUBIC_ENGINE_TYPES_H

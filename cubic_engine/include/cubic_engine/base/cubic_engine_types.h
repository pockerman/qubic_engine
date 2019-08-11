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

    /// \brief General vector type
    template<typename T>
    using DynVec = blaze::DynamicVector<T>;

    /// \brief Identity matrix
    template<typename T>
    using IdentityMatrix = blaze::IdentityMatrix<T>;

    /// \brief basic primitive data types
    using real_t = kernel::real_t;
    using uint_t = kernel::uint_t;

}

#endif // CUBIC_ENGINE_TYPES_H

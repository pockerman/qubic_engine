#ifndef CUBIC_ENGINE_TYPES_H
#define CUBIC_ENGINE_TYPES_H

#include <cstddef>
#include <blaze/Math.h>


namespace cengine
{

    /// \brief General matrix type
    template<typename T>
    using DynMat = blaze::DynamicMatrix<T>;

    /// \brief General vector type
    template<typename T>
    using DynVec = blaze::DynamicVector<T>;

    using real_t = double;
    using uint_t = std::size_t;

}

#endif // CUBIC_ENGINE_TYPES_H

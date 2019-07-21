#ifndef SCALED_SUM_H
#define SCALED_SUM_H

namespace kernel
{

/**
 *Returns the scaled sum of the two entities
 */
template<typename T, typename F=T>
struct ScaledSum
{
    static const T get(const T& x1, const F& f1, const T& x2, const F& f2){return x1*f1 + x2*f2;}
};

template<typename T, typename F=T>
struct ScaledDif
{
    static const T get(const T& x1, const F& f1, const T& x2, const F& f2){return x1*f1 - x2*f2;}
};

}

#endif // SCALED_SUM_H

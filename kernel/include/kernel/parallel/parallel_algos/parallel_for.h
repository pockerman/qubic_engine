#ifndef PARALLEL_FOR_H
#define PARALLEL_FOR_H

namespace kernel
{

namespace detail
{

}

/// \brief Apply Body on the elements of Range
/// The application of the Body on Range in handled by the Executor type
template<typename Range, typename Body, typename Executor>
void parallel_for(Range& range, const Body& op, Executor& executor){

}
}

#endif // PARALLEL_FOR_H

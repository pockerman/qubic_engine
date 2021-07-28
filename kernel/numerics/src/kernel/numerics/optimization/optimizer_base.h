#ifndef OPTIMIZER_BASE_H
#define OPTIMIZER_BASE_H

#include "kernel/base/types.h"
#include "kernel/parallel/utilities/result_holder.h"
#include "kernel/utilities/algorithm_info.h"
#include <boost/noncopyable.hpp>

namespace kernel {

template<typename OutputTp, typename... InputTp>
class FunctionBase;

namespace numerics {
namespace opt {

template<typename MatTp, typename VecTp>
class OptimizerBase: private boost::noncopyable
{

public:

    typedef MatTp data_set_t;
    typedef VecTp labels_set_t;
    typedef FunctionBase<ResultHolder<real_t>, MatTp, VecTp> function_t;

    ///
    /// \brief Expose the type that is returned by this object
    /// when calling its solve functions
    ///
    typedef AlgInfo output_t;

    ///
    ///
    ///
    virtual ~OptimizerBase()=default;

    ///
    /// \brief solve
    /// \param features
    /// \param labels
    /// \param fnc
    ///
    virtual output_t solve(const data_set_t& features, const labels_set_t& labels, function_t& fnc) = 0;

protected:

    ///
    ///
    ///
    OptimizerBase()=default;

};
}
}
}

#endif // OPTIMIZER_BASE_H

#ifndef OPTIMIZER_BASE_H
#define OPTIMIZER_BASE_H

#include "kernel/base/types.h"
#include "kernel/parallel/utilities/result_holder.h"
#include "kernel/utilities/iterative_algorithm_result.h"
#include "kernel/numerics/optimization/optimizer_type.h"

#include <boost/noncopyable.hpp>

namespace kernel {

template<typename OutputTp, typename... InputTp>
class FunctionBase;

namespace numerics {
namespace opt {

template<typename DatasetTp, typename FunctionTp>
class OptimizerBase: private boost::noncopyable
{

public:

    ///
    /// \brief data_set_t
    ///
    typedef DatasetTp data_set_t;

    ///
    /// \brief function_t
    ///
    typedef FunctionTp function_t;

    ///
    /// \brief Expose the type that is returned by this object
    /// when calling its solve functions
    ///
    typedef IterativeAlgorithmResult output_t;

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
    virtual output_t solve(const data_set_t& points, function_t& fnc) = 0;

    ///
    /// \brief type
    /// \return
    ///
    virtual OptimizerType type()const=0;

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

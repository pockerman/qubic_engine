#ifndef TORCH_GENERATOR_BASE_H
#define TORCH_GENERATOR_BASE_H

#include "kernel/base/config.h"

#ifdef USE_PYTORCH

#include "cubic_engine/rl/utils/torch_mini_batch.h"
#include "boost/noncopyable.hpp"

namespace cengine {
namespace rl {
namespace utils {



class TorchGeneratorBase: private boost::noncopyable{

public:

    ///
    ///
    ///
    virtual ~TorchGeneratorBase() = default;

    ///
    /// \brief done
    /// \return
    ///
    virtual bool done() const = 0;

    ///
    /// \brief next
    /// \return
    ///
    virtual TorchMiniBatch next() = 0;

protected:

    ///
    ///
    ///
    TorchGeneratorBase() = default;


};

}

}

}

#endif
#endif // TORCH_GENERATOR_BASE_H

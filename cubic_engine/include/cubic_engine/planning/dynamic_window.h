#ifndef DYNAMIC_WINDOW_H
#define DYNAMIC_WINDOW_H

#include "cubic_engine/base/cubic_engine_types.h"

namespace cengine{
namespace planning {


template<typename StateTp, typename ConfigTp>
class DynamicWindowBase{

public:

    ///
    /// \brief state_t. The type of the state used by the window
    ///
    typedef StateTp state_t;

    ///
    /// \brief config_t. Congiguration type used by the window
    ///
    typedef ConfigTp config_t;

    ///
    /// \brief ~DynamicWindowBase. Destructor
    ///
    virtual ~DynamicWindowBase(){}

    ///
    /// \brief calculate_window. Calculate the window
    ///
    virtual void calculate_window()=0;

protected:

    ///
    /// \brief DynamicWindow. Constructor
    ///
    DynamicWindowBase(state_t& state, const config_t& config);

    ///
    /// \brief state_. Pointer to the state that the
    /// window is using.
    ///
    state_t* state_;

    ///
    /// \brief config_. The congiration of thw window
    /// for Window calculation
    ///
    config_t config_;

};


template<typename StateTp, typename ConfigTp>
DynamicWindowBase<StateTp, ConfigTp>::DynamicWindowBase(state_t& state, const config_t& config)
    :
      state_(&state),
      config_(config)
{}

}
}

#endif // DYNAMIC_WINDOW_H

#ifndef BANG_BANG_PATH_TRACK_CONTROLER_H
#define BANG_BANG_PATH_TRACK_CONTROLER_H

#include "cubic_engine/base/cubic_engine_types.h"
namespace cengine {
namespace control {

/// \brief Implements the path track controller
/// descibed in
/// A Smooth Path Tracking Algorithm for Wheeled
/// Mobile Robots with Dynamic Constraints
/// Journal of Intelligent and Robotic Systems
/// proposed by K. C. KOH and H. S. CHO
///
///
class BangBangPathTrackController
{
public:


    /// \brief Execute controller
    void execute()const;


private:

    /// \brief The fitting coefficient
    real_t cx_;

    /// \brief Maximum acceleration
    real_t amax_;
};

}

}

#endif // BANG_BANG_PATH_TRACK_CONTROLER_H

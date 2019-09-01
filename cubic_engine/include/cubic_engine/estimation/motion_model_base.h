#ifndef MOTION_MODEL_BASE_H
#define MOTION_MODEL_BASE_H

namespace cengine
{

template<typename VecTp, typename MatTp>
class MotionModelBase
{

public:

    typedef VecTp vector_type;
    typedef MatTp matrix_type;

    MotionModelBase() = default;
    virtual ~MotionModelBase() = default;

    /// \brief By deafult computes x = A*x_prev + B*u
    virtual VecTp operator()(const VecTp& x_prev, const MatTp& A, const MatTp& B, const VecTp& u  )const;

};

template<typename VecTp, typename MatTp>
VecTp
MotionModelBase<VecTp, MatTp>::operator()(const VecTp& x_prev, const MatTp& A, const MatTp& B, const VecTp& u  )const{
    return A*x_prev + B*u;
}

}

#endif // MOTION_MODEL_H

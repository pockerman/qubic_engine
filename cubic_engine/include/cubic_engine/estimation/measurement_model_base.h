#ifndef MEASUREMENT_MODEL_BASE_H
#define MEASUREMENT_MODEL_BASE_H

namespace cengine
{

template<typename VecTp, typename MatTp>
class MeasurementModelBase
{

public:

    typedef VecTp vector_type;
    typedef MatTp matrix_type;

    MeasurementModelBase() = default;
    virtual ~MeasurementModelBase() = default;

    /// \brief By deafult computes z = H*x
    virtual VecTp operator()(const VecTp& x, const MatTp& H)const;

};

template<typename VecTp, typename MatTp>
VecTp
MeasurementModelBase<VecTp, MatTp>::operator()(const VecTp& x, const MatTp& H )const{
    return H*x;
}

}

#endif // MEASUREMENT_MODEL_BASE_H

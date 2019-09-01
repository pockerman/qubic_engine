#ifndef EXE_H
#define EXE_H

#include "cubic_engine/base/cubic_engine_types.h"
#include "cubic_engine/estimation/particle_filter.h"
#include "cubic_engine/estimation/motion_model_base.h"
#include "cubic_engine/estimation/measurement_model_base.h"
#include "cubic_engine/statistics/gauss_likelihood_model.h"
#include "kernel/utilities/csv_file_writer.h"

namespace exe
{

// forward declarations
struct Input;

using cengine::real_t;
using cengine::uint_t;
using cengine::DynMat;
using cengine::DynVec;
using cengine::ParticleFilter;
using cengine::ParticleFilterMatrixDescriptor;
using cengine::MeasurementModelBase;
using cengine::MotionModelBase;
using cengine::GaussLikelihood;
using kernel::CSVWriter;


typedef DynVec<real_t> state_type;
typedef MotionModelBase<DynVec<real_t> , DynMat<real_t> > motion_model_type;
typedef MeasurementModelBase<DynVec<real_t> , DynMat<real_t> > measurement_model_type;
typedef ParticleFilterMatrixDescriptor matrix_descriptor_type;
typedef GaussLikelihood<real_t, real_t> likelihood_model_type;
typedef Input input_type;

const uint_t N_STEPS = 500;

struct SimData
{

    uint_t n_steps;
    uint_t num_parts;
    uint_t num_resample_parts;
};

struct Input
{
    typedef DynVec<real_t> control_type;
    typedef DynVec<real_t> measurement_type;

    // the measurement vector
    DynVec<real_t> y;

    //the input vector
    DynVec<real_t> u;

    Input();
    const DynVec<real_t>& get_control()const{return u;}
    const DynVec<real_t>& get_measurement()const{return y;}
};

inline
Input::Input()
    :
      y(2, 0.0),
      u(2, 0.0)
{
    u[0] = 1.0; //m/s
    u[1] = 0.1; //rad/s
}


class Robot
{
public:



    // contructor
    Robot(const SimData& data);


    void simulate(const Input& in){}

    // save the current state
    void save_state(CSVWriter& writer)const{}


private:

    // the PF estimator
    ParticleFilter<state_type, input_type, motion_model_type, measurement_model_type, likelihood_model_type, matrix_descriptor_type > pf_;
};
}

#endif // EXE_H

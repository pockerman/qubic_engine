#ifndef EXE_H
#define EXE_H

#include "kernel/base/config.h"

#ifdef USE_LOG
#include "kernel/utilities/logger.h"
#endif

#include "kernel/data_structs/boost_serial_graph.h"
#include "kernel/discretization/element.h"
#include "kernel/discretization/quad_mesh_generation.h"
#include "kernel/discretization/mesh.h"
#include "kernel/discretization/line_mesh.h"

#include "kernel/base/types.h"
#include "kernel/parallel/threading/thread_pool.h"
#include "kernel/parallel/threading/stoppable_task.h"
#include "kernel/parallel/data_structs/lockable_queue.h"
#include "kernel/vehicles/difd_drive_vehicle.h"
#include "kernel/geometry/geom_point.h"
#include "kernel/utilities/csv_file_writer.h"
#include "kernel/patterns/threaded_observer_base.h"
#include "kernel/dynamics/system_state.h"
#include "kernel/dynamics/diff_drive_dynamics.h"

#include "cubic_engine/base/cubic_engine_types.h"
#include "cubic_engine/estimation/extended_kalman_filter.h"
#include "cubic_engine/grids/waypoint_path.h"
#include "cubic_engine/control/carrot_chasing.h"

#include <iostream>
#include <memory>
#include <mutex>
#include <thread>
#include <limits>
#include <vector>
#include <condition_variable>

namespace example{

using cengine::real_t;
using cengine::uint_t;
using cengine::estimation::ExtendedKalmanFilter;
using cengine::control::CarrotChasingPathTrackController;
using cengine::grids::WaypointPath;
using cengine::grids::WayPoint;
using cengine::grids::LineSegmentData;
using cengine::control::CarrotChasingPathTrackControllerInput;
using DynMat = cengine::DynMat<real_t>;
using DynVec = cengine::DynVec<real_t>;

using kernel::ThreadPool;
using kernel::ThreadPoolOptions;
using kernel::StoppableTask;
using kernel::LockableQueue;
using kernel::GeomPoint;
using kernel::numerics::Mesh;
using kernel::BoostSerialGraph;
using kernel::Null;
using kernel::ThreadedObserverBase;
using kernel::dynamics::SysState;
using kernel::dynamics::DiffDriveDynamics;
//using kernel::DiffDriveProperties;
using kernel::DiffDriveVehicle;

std::mutex msg_mutex;

const real_t DT = 0.1;
const std::string SET_GOAL("SET_GOAL");
const std::string SET_GOAL_SERVER("SET_GOAL_SERVER");
const std::string GOAL("GOAL");
const std::string PRINT("PRINT");
const std::string PRINT_GOAL("PRINT_GOAL");
const std::string EXIT("EXIT");
const std::string RESPONSE("RESPONSE: ");
const std::string MESSAGE("MESSAGE: ");
const std::string EMPTY_CMD("");
const std::string ENTER_CMD("ENTER_CMD");
const std::string V_CMD_SERVER("V_CMD_SERVER");
const std::string V_CMD("V_CMD");
const std::string W_CMD_SERVER("W_CMD_SERVER");
const std::string W_CMD("W_CMD");

/// gloabl variable to synchronize stop
std::atomic<bool> threads_should_stop(false);

/// global variable indicating whether the goal is reached
std::atomic<bool> goal_reached(false);

/// cycle of path constructor thread
/// in milliseconds
const uint_t PATH_CONSTRUCTOR_CYCLE = 30;

/// cycle of state estimation thread
const uint_t STATE_ESTIMATION_THREAD_CYCLE = 25;

/// cycle for path correction thread
const uint_t PATH_CORRECTION_THREAD_CYCLE = 5;


///vertex data to apply A*
struct AstarNodeData
{
    real_t gcost;
    real_t fcost;
    real_t occumancy_prob;

    GeomPoint<2> position;

    AstarNodeData();
    AstarNodeData(const GeomPoint<2>& p);
    AstarNodeData(const AstarNodeData& o);
    bool can_move()const{return occumancy_prob != 1.0;}

};

AstarNodeData::AstarNodeData()
:
gcost(std::numeric_limits<real_t>::max()),
fcost(std::numeric_limits<real_t>::max()),
position()
{}

AstarNodeData::AstarNodeData(const GeomPoint<2>& p)
:
gcost(std::numeric_limits<real_t>::max()),
fcost(std::numeric_limits<real_t>::max()),
position(p)
{}

AstarNodeData::AstarNodeData(const AstarNodeData& o)
:
gcost(o.gcost),
fcost(o.fcost),
position(o.position)
{}

/// class that implements the observation model
class ObservationModel
{

public:

    typedef  DynVec input_t;

    /// constructor
    ObservationModel();

    /// simply return th
    const DynVec evaluate(const DynVec& input)const;

    /// get the H or M matrix
    const DynMat& get_matrix(const std::string& name)const;

    /// initialize the matrices
    void initialize_matrices();

private:

    DynMat H_;
    DynMat M_;
};

inline
ObservationModel::ObservationModel()
    :
      H_(2,3,0.0),
      M_(2,2,0.0)
{}

typedef BoostSerialGraph<AstarNodeData, LineSegmentData> Map;
typedef WaypointPath<2, AstarNodeData, LineSegmentData> Path;
typedef SysState<5> State;
typedef GeomPoint<2> Goal;
typedef DiffDriveDynamics MotionModel;


/// helper classes
struct StopSimulation
{
    bool stop()const{return condition; }
    void set_condition(bool cond){condition=cond;}
    bool condition{false};
};

class RefVelocityObserver: public ThreadedObserverBase<std::mutex, real_t>
{
public:

    typedef ThreadedObserverBase<std::mutex, real_t>::resource_t ref_velocity_resource_t;

    virtual void update(const ref_velocity_resource_t& resource)override final;
    virtual void read(ref_velocity_resource_t&)const override final;

};

class StateObserver: public ThreadedObserverBase<std::mutex, State>
{

public:

    typedef ThreadedObserverBase<std::mutex, State>::resource_t state_resource_t;

    virtual void update(const state_resource_t& resource)override final;
    virtual void read(state_resource_t&)const override final;

private:

  /// the cv to wait unitl state is ready
  mutable std::condition_variable cv;
};

class GoalObserver: public ThreadedObserverBase<std::mutex, Goal>
{

public:

    typedef ThreadedObserverBase<std::mutex, Goal>::resource_t goal_resource_t;

    virtual void update(const goal_resource_t& resource)override final;
    virtual void read(goal_resource_t&)const override final;
};

class PathObserver: public ThreadedObserverBase<std::mutex, Path*>
{
public:

    typedef ThreadedObserverBase<std::mutex, Path*>::resource_t path_resource_t;


    virtual void update(const path_resource_t& resource)override final;
    virtual const path_resource_t& read()const override final;

private:

  /// the cv to wait unitl path is ready
  mutable std::condition_variable cv;

};



/// Uses EKF algorithm to provide an estimation
/// of the robot state
struct StateEstimationThread: public kernel::StoppableTask<StopSimulation>

{
public:

    RefVelocityObserver vobserver;
    RefVelocityObserver wobserver;
    
    /// Constructor
    StateEstimationThread(const StopSimulation& stop, const Map& map);

    /// attach a state observer
    void attach_state_observer(StateObserver& sobserver){sobservers_.push_back(&sobserver);}

    /// update the path observers that a new path is ready
    void update_state_observers();

    /// run the thread
    virtual void run()override final;

    /// initialize the object
    void initialize();

    /// set the initial state for the motion
    void set_state(const State& state);

private:

    
    /// the state of the system
    State state_;

    /// the motion model used by the state estimator
    MotionModel m_model_;

    /// the observation model the state estimator is using
    ObservationModel o_model_;

    /// the EKF filter
    ExtendedKalmanFilter<MotionModel, ObservationModel> ekf_;

    ///  the observer list
    std::vector<StateObserver*> sobservers_;

    /// pointer to the map
    const Map* map_;

    /// the velocity control
    real_t v_ctrl_;

    /// the angular velocity control
    real_t w_ctrl_;
};

inline
StateEstimationThread::StateEstimationThread(const StopSimulation& stop, const Map& map)
    :
    StoppableTask<StopSimulation>(stop),
    vobserver(),
    wobserver(),
    state_(),
    m_model_(),
    o_model_(),
    ekf_(m_model_, o_model_),
    sobservers_(),
    map_(&map),
    v_ctrl_(0.0),
    w_ctrl_(0.0)
{
    this->set_name("StateEstimationThread");

    vobserver.set_name("StateEstimationThread V Observer");
    wobserver.set_name("StateEstimationThread W Observer");

    /// create the requests and report the initial state
    state_.set(0, {"X", 0.0});
    state_.set(1, {"Y", 0.0});
    state_.set(2, {"Theta", 0.0});
    state_.set(3, {"V", 0.0});
    state_.set(4, {"W", 0.0});
}


/// Given the state produced by the
/// StateEstimationThread produce a Path
/// to be followed using A* lagorithm
struct PathConstructorThread: public StoppableTask<StopSimulation>
{
public:

    /// The observer for the goal
    GoalObserver gobserver;

    /// The observer for the state
    StateObserver sobserver;

    /// constructor
    PathConstructorThread(const StopSimulation& stop, const Map& map);

    /// attach a Path observer
    void attach_path_observer(PathObserver& pobserver){pobservers_.push_back(&pobserver);}

    /// update the path observers that a new path is ready
    void update_path_observers();

protected:

    /// run the thread
    virtual void run()override final;

    /// the map used
    const Map* map_;

    /// the path constructed
    Path path_;

    /// the observers to notify on the path update
    std::vector<PathObserver*> pobservers_;

    /// save the computed path
    void save_path(real_t duration);

};

inline
PathConstructorThread::PathConstructorThread(const StopSimulation& stop,
                                             const Map& map)
    :
    StoppableTask<StopSimulation>(stop),
    gobserver(),
    sobserver(),
    map_(&map),
    path_()
{
    this->set_name("PathConstructorThread");
    gobserver.set_name("PathConstructorThread GOAL Observer");
    sobserver.set_name("PathConstructorThread STATE Observer");

}

/// Given the Path produced by the  PathConstructorThread
/// creates motion commands so that the robot
/// follows the specified path
struct PathFollowerThread: public StoppableTask<StopSimulation>
{

    /// The observer for the goal
    GoalObserver gobserver;

    /// Path observer for output
    PathObserver pobserver;

    /// The state observer
    StateObserver sobserver;

    /// constructor
    PathFollowerThread(const StopSimulation& stop,
                       const CarrotChasingPathTrackControllerInput& input,
                       real_t gradius);

    /// attach an angular velocity observer
    void attach_w_velocity_observer(RefVelocityObserver& observer);

    /// update the w velocity observers
    void update_w_velocity_observers(real_t w);

    /// set the initial state for the motion
    void set_state(const State& state){state_ = state;}

protected:

    /// run the thread
    virtual void run()override final;

    /// The path tracker used
    CarrotChasingPathTrackController<AstarNodeData, LineSegmentData> path_controller_;

    /// the list of angular observers to update
    std::vector<RefVelocityObserver*> wobservers_;

    /// the goal radius
    real_t gradius_;

    /// pointer to the path
    const Path* path_;

    /// the state of the system
    State state_;
};

inline
PathFollowerThread::PathFollowerThread(const StopSimulation& stop,
                                       const CarrotChasingPathTrackControllerInput& input,
                                       real_t gradius)
    :
   StoppableTask<StopSimulation>(stop),
   gobserver(),
   sobserver(),
   pobserver(),
   path_controller_(input),
   wobservers_(),
   gradius_(gradius),
   path_(nullptr),
   state_()
{
    this->set_name("PathFollowerThread");
    gobserver.set_name("PathFollowerThread GOAL Observer");
    sobserver.set_name("PathFollowerThread STATE Observer");
    pobserver.set_name("PathFollowerThread PATH Observer");

    /// create the requests and report the initial state
    state_.set(0, {"X", 0.0});
    state_.set(1, {"Y", 0.0});
    state_.set(2, {"Theta", 0.0});
    state_.set(3, {"V", 0.0});
    state_.set(4, {"W", 0.0});
}


inline
void
PathFollowerThread::attach_w_velocity_observer(RefVelocityObserver& observer){
    wobservers_.push_back(&observer);
}

inline
void
PathFollowerThread::update_w_velocity_observers(real_t w){
    for(auto observer:wobservers_){
        observer->update(w);
    }

    //std::lock_guard<std::mutex> lock(msg_mutex);
    //std::cout<<"MESSAGE: Task "+this->get_name()<<" updated W velocity observers"<<std::endl;
}

}

#endif // EXE_H

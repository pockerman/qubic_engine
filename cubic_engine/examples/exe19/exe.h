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

namespace example{

std::mutex msg_mutex;

using cengine::real_t;
using cengine::uint_t;
using cengine::ExtendedKalmanFilter;
using cengine::control::CarrotChasingPathTrackController;
using cengine::grids::WaypointPath;
using cengine::grids::WayPoint;
using cengine::grids::LineSegmentData;
using DynMat = cengine::DynMat<real_t>;
using DynVec = cengine::DynVec<real_t>;

using kernel::ThreadPool;
using kernel::ThreadPoolOptions;
using kernel::StoppableTask;
using kernel::LockableQueue;
using kernel::GeomPoint;
using kernel::numerics::Mesh;
using kernel::numerics::LineMesh;
using kernel::BoostSerialGraph;
using kernel::Null;
using kernel::ThreadedObserverBase;
using kernel::dynamics::SysState;
using kernel::dynamics::DiffDriveDynamics;
using kernel::DiffDriveProperties;
using kernel::DiffDriveVehicle;

const real_t DT = 0.5;
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
const std::string V_CMD("V_CMD");
const std::string W_CMD("W_CMD");

/// gloabl variable to synchronize stop
std::atomic<bool> threads_should_stop(false);

/// cycle of path constructor thread
/// in milliseconds
const uint_t PATH_CONSTRUCTOR_CYCLE = 10;

/// cycle of requests thread
/// in milliseconds
const uint_t REQUESTS_THREAD_CYCLE = 10;

/// cycle of state estimation thread
const uint_t STATE_ESTIMATION_THREAD_CYCLE = 10;

/// cycle for path correction thread
const uint_t PATH_CORRECTION_THREAD_CYCLE = 10;

/// cycle for client thread
/// in milliseconds
const uint_t CLIENT_THREAD_CYCLE = 10;

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

    ObservationModel();

    /// simply return th
    const DynVec evaluate(const DynVec& input)const{}

    /// get the H or M matrix
    const DynMat& get_matrix(const std::string& name)const{}

private:

    DynMat H;
    DynMat M;
};

inline
ObservationModel::ObservationModel()
    :
      H(),
      M()
{}

typedef BoostSerialGraph<AstarNodeData, LineSegmentData> Map;
typedef WaypointPath<2, AstarNodeData, LineSegmentData> Path;
typedef SysState<4> State;
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

class MeasurmentObserver: public ThreadedObserverBase<std::mutex, DynVec>
{
public:

    typedef ThreadedObserverBase<std::mutex, DynVec>::resource_t measurement_resource_t;

    virtual void update(const measurement_resource_t& resource)override final;
    virtual void read(measurement_resource_t&)const override final;

};

class StateObserver: public ThreadedObserverBase<std::mutex, State>
{

public:

    typedef ThreadedObserverBase<std::mutex, State>::resource_t state_resource_t;

    virtual void update(const state_resource_t& resource)override final;
    virtual void read(state_resource_t&)const override final;
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
};

struct CMD
{
    real_t velocity{0.0};
    real_t orientation{0.0};
    std::string name{"INVALID_CMD"};

    CMD()=default;

    CMD(real_t v, real_t o, std::string n)
        :
          velocity(v),
          orientation(o),
          name(n)
    {}

    static CMD generate_cmd(real_t v, real_t o, std::string name){
        return CMD(v, o, name);
    }
};


class ServerThread: public kernel::StoppableTask<StopSimulation>
{
public:

    GoalObserver gobserver;

    /// constructor
    ServerThread(const StopSimulation& stop_condition,
                 const Map& map,
                 const SysState<4>& state,
                 ThreadPool& threads);

    /// run the server
    virtual void run()override final;

    /// Query the sensor interface for measurements
    const DynVec get_measurement()const;

    /// attach any measurement observers to update
    /// when the measurement is taken
    void attach_measurement_observer(MeasurmentObserver& observer){m_observers_.push_back(&observer);}

    /// update the observers with the new measurement
    void update_measurement_observers(MeasurmentObserver::measurement_resource_t& measurement);

protected:

    /// The map used
    const Map* map_;

    /// the initial state
    State init_state_;

    LockableQueue<std::string> requests_;
    LockableQueue<std::string> responses_;
    LockableQueue<CMD> cmds_;

    /// the thread pool
    ThreadPool& thread_pool_;

    /// list of tasks the server handles
    std::vector<std::unique_ptr<kernel::TaskBase>> tasks_;

    /// observers that we should update when a measurement
    /// of the sensor is taken
    std::vector<MeasurmentObserver*> m_observers_;

    /// checks if all tasks are stopped
    /// if this is true the server is stopped as well
    void tasks_stopped();

    struct RequestTask;
    struct ClientTask;
    struct StateEstimationThread;
    struct PathConstructorThread;
    struct PathFollowerThread;

};


struct ServerThread::RequestTask: public kernel::StoppableTask<StopSimulation>
{

public:

    /// Goal Observer
    GoalObserver gobserver;

    /// State observer
    StateObserver sobserver;

    /// Path observer for output
    PathObserver pobserver;

    /// Reference velocity requested
    RefVelocityObserver vobserver;

    /// Reference angular velocity requested
    RefVelocityObserver wobserver;

    /// constructor
    RequestTask(const StopSimulation& stop_condition,
                LockableQueue<std::string>& requests,
                LockableQueue<CMD>& cmds,
                LockableQueue<std::string>& responses);

    /// update the goal for the local
    /// gobserver and the observers subscribed
    void update_goal_observers(const Goal& goal);

    /// attach a goal observer
    void attach_goal_observer(GoalObserver& observer){goal_observers_.push_back(&observer);}

    /// update the velocity observers
    void update_v_observers(real_t v);

    /// attach a velocity observer
    void attach_v_observer(RefVelocityObserver& observer){v_observers_.push_back(&observer);}

    /// update the w velocity observers
    void update_w_observers(real_t w);

    /// attach a velocity observer
    void attach_w_observer(RefVelocityObserver& observer){w_observers_.push_back(&observer);}

protected:

    virtual void run()override final;
    LockableQueue<std::string>& requests_;
    LockableQueue<CMD>& cmds_;
    LockableQueue<std::string>& responses_;

    ///  The observer list for the goal
    std::vector<GoalObserver*> goal_observers_;

    /// The observer list for the reference velocity
    std::vector<RefVelocityObserver*> v_observers_;

    /// The observer list for the angular velocity
    std::vector<RefVelocityObserver*> w_observers_;

    void serve_request(const std::string& request);
    void save_solution(kernel::CSVWriter& writer);
};

inline
ServerThread::RequestTask::RequestTask(const StopSimulation& stop_condition,
                                       LockableQueue<std::string>& requests,
                                       LockableQueue<CMD>& cmds,
                                       LockableQueue<std::string>& responses)
    :
kernel::StoppableTask<StopSimulation>(stop_condition),
gobserver(),
sobserver(),
pobserver(),
vobserver(),
wobserver(),
requests_(requests),
cmds_(cmds),
responses_(responses)
{
    this->set_name("RequestTask");
    gobserver.set_name("ReqestTask GOAL Observer");
    sobserver.set_name("RequestTask STATE Observer");
    pobserver.set_name("RequestTask PATH Observer");
}

struct ServerThread::ClientTask: public kernel::StoppableTask<StopSimulation>
{

public:

    ClientTask(const StopSimulation& stop_condition,
               LockableQueue<std::string>& request,
               LockableQueue<std::string>& response);

    virtual void run()override final;
protected:


    LockableQueue<std::string>& requests_;
    LockableQueue<std::string>& responses_;

};

inline
ServerThread::ClientTask::ClientTask(const StopSimulation& stop_condition,
                                     LockableQueue<std::string>& request,
                                     LockableQueue<std::string>& response)
    :
  kernel::StoppableTask<StopSimulation>(stop_condition),
  requests_(request),
  responses_(response)

{
    this->set_name("ClientTask");
}

/// Uses EKF algorithm to provide an estimation
/// of the robot state
struct ServerThread::StateEstimationThread: public kernel::StoppableTask<StopSimulation>

{
public:

    RefVelocityObserver vobserver;
    RefVelocityObserver wobserver;
    MeasurmentObserver  mobserver;

    /// Constructor
    StateEstimationThread(const StopSimulation& stop, const Map& map);

    /// attach a state observer
    void attach_state_observer(StateObserver& sobserver){sobservers_.push_back(&sobserver);}

    /// update the path observers that a new path is ready
    void update_state_observers();

private:

    /// run the thread
    virtual void run()override final;

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
};

inline
ServerThread::StateEstimationThread::StateEstimationThread(const StopSimulation& stop, const Map& map)
    :
    StoppableTask<StopSimulation>(stop),
    vobserver(),
    wobserver(),
    mobserver(),
    state_(),
    m_model_(),
    o_model_(),
    ekf_(m_model_, o_model_),
    sobservers_(),
    map_(&map)
{
    this->set_name("StateEstimationThread");

    /// create the requests and report the initial state
    state_.set(0, {"x", 0.0});
    state_.set(1, {"y", 0.0});
    state_.set(2, {"V", 0.0});
    state_.set(3, {"W", 0.0});
}


/// Given the state produced by the
/// StateEstimationThread produce a Path
/// to be followed using A* lagorithm
struct ServerThread::PathConstructorThread: public StoppableTask<StopSimulation>
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
ServerThread::PathConstructorThread::PathConstructorThread(const StopSimulation& stop,
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
struct ServerThread::PathFollowerThread: public StoppableTask<StopSimulation>
{

    /// The observer for the goal
    GoalObserver gobserver;

    /// The observer for the Path
    PathObserver pobserver;

    /// The state observer
    StateObserver sobserver;

    // constructor
    PathFollowerThread(const StopSimulation& stop,
                       LockableQueue<std::string>& responses,
                       real_t gradius,
                       real_t gain);

protected:

    /// run the thread
    virtual void run()override final;

    /// The path tracker used
    CarrotChasingPathTrackController<Null, LineSegmentData> path_controller_;

    /// The goal radius
    real_t gradius_;

    /// The responses queue to notify when the
    /// goal hs been reached
    LockableQueue<std::string>& responses_;
};

inline
ServerThread::PathFollowerThread::PathFollowerThread(const StopSimulation& stop,
                                                     LockableQueue<std::string>& responses,
                                                     real_t gradius,
                                                     real_t gain)
    :
   StoppableTask<StopSimulation>(stop),
   gobserver(),
   pobserver(),
   sobserver(),
   path_controller_(),
   gradius_(gradius),
   responses_(responses)
{
    this->set_name("PathFollowerThread");
    gobserver.set_name("PathFollowerThread GOAL Observer");
    sobserver.set_name("PathFollowerThread STATE Observer");
    pobserver.set_name("PathFollowerThread PATH Observer");
}


inline
ServerThread::ServerThread(const StopSimulation& stop_condition,
                           const Map& map,
                           const SysState<4>& init_state,
                           ThreadPool& thread_pool)
    :
  kernel::StoppableTask<StopSimulation>(stop_condition),
  map_(&map),
  init_state_(init_state),
  requests_(),
  responses_(),
  cmds_(),
  thread_pool_(thread_pool),
  tasks_()
{
    this->set_name("ServerThread");
}

}

#endif // EXE_H

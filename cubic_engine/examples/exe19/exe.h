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

#include "cubic_engine/base/cubic_engine_types.h"
#include "cubic_engine/estimation/extended_kalman_filter.h"


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
using DynMat = cengine::DynMat<real_t>;
using DynVec = cengine::DynVec<real_t>;
using kernel::ThreadPool;
using kernel::ThreadPoolOptions;
using kernel::StoppableTask;
using kernel::DiffDriveVehicle;
using kernel::LockableQueue;
using kernel::GeomPoint;
using kernel::numerics::Mesh;
using kernel::numerics::LineMesh;
using kernel::BoostSerialGraph;
using kernel::Null;
using kernel::ThreadedObserverBase;
using kernel::dynamics::SysState;

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

/// gloabl variable to synchronize stop
std::atomic<bool> threads_should_stop(false);

/// cycle of each thread
const uint_t PATH_CONSTRUCTOR_CYCLE = 1;

//vertex data to apply A*
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

typedef BoostSerialGraph<AstarNodeData, Null> Map;
typedef LineMesh<2> Path;
typedef SysState<4> State;
typedef GeomPoint<2> Goal;

// helper classes
struct StopSimulation
{
    bool stop()const{return condition; }
    void set_condition(bool cond){condition=cond;}
    bool condition{false};
};

class StateObserver: public ThreadedObserverBase<std::mutex, State>
{

public:

    typedef ThreadedObserverBase<std::mutex, State>::resource_t state_resource_t;

    // update the reource
    virtual void update(const state_resource_t& resource)override final;

    // read the resource
    virtual void read(state_resource_t&)const override final;
};

class GoalObserver: public ThreadedObserverBase<std::mutex, Goal>
{

public:

    typedef ThreadedObserverBase<std::mutex, Goal>::resource_t goal_resource_t;

    // update the reource
    virtual void update(const goal_resource_t& resource)override final;

    // read the resource
    virtual void read(goal_resource_t&)const override final;
};

class PathObserver: public ThreadedObserverBase<std::mutex, Path*>
{
public:

    typedef ThreadedObserverBase<std::mutex, Path*>::resource_t path_resource_t;

    // update the reource
    virtual void update(const path_resource_t& resource)override final;

    // read the resource
    virtual void read(path_resource_t&)const override final;
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


class DiffDriveVehicleWrapper
{

public:

    DiffDriveVehicleWrapper(DiffDriveVehicle& vehicle);

    /// set the CMD to execute
    void set_cmd(CMD cmd);

    /// \brief integrate the diff drive system
    /// account for errors also
    std::string integrate();

    /// \brief Returns the state of the vehicle as string
    std::string get_state_as_string()const;

    /// \brief Returns the state of the vehicle as string
    std::tuple<real_t, real_t> get_position()const;

    /// \brief Returns the state of the vehicle as string
    std::tuple<real_t, real_t, real_t, real_t> get_state()const;

private:

    mutable std::mutex integrate_mutex_;
    DiffDriveVehicle* vehicle_ptr_;
    CMD cmd_;
};


class ServerThread: public kernel::StoppableTask<StopSimulation>
{
public:

    GoalObserver gobserver;

    // constructor
    ServerThread(const StopSimulation& stop_condition,
                 const Map& map,
                 const SysState<4>& state,
                 DiffDriveVehicleWrapper& vwrapper,
                 ThreadPool& threads);


    // run the server
    virtual void run()override final;

protected:

    // The map used
    const Map* map_;

    // the initial state
    State init_state_;

    // wrapper to the vehicle model
    DiffDriveVehicleWrapper& vwrapper_;

    LockableQueue<std::string> requests_;
    LockableQueue<std::string> responses_;
    LockableQueue<CMD> cmds_;

    ThreadPool& thread_pool_;


    // list of tasks the server handles
    std::vector<std::unique_ptr<kernel::TaskBase>> tasks_;

    // checks if all tasks are stopped
    // if this is true the server is stopped as well
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

    //Goal Observer
    GoalObserver gobserver;

    //State observer
    StateObserver sobserver;

    // Path observer for output
    PathObserver pobserver;

    RequestTask(const StopSimulation& stop_condition,
                LockableQueue<std::string>& requests,
                LockableQueue<CMD>& cmds,
                LockableQueue<std::string>& responses);

    // update the goal for the local
    // gobserver and the observers subscribed
    void update_goal_observers(const Goal& goal);

    // attach a goal observer
    void attach_goal_observer(GoalObserver& observer){goal_observers_.push_back(&observer);}

protected:

    virtual void run()override final;
    LockableQueue<std::string>& requests_;
    LockableQueue<CMD>& cmds_;
    LockableQueue<std::string>& responses_;

    /// \brief The observer list for the goal
    std::vector<GoalObserver*> goal_observers_;


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

    //virtual void run()override final;
    LockableQueue<std::string>& requests_;
    LockableQueue<std::string>& responses_;

};

// Uses EKF algorithm to provide an estimation
// of the robot state
struct ServerThread::StateEstimationThread: public kernel::StoppableTask<StopSimulation>

{
public:

    // Constructor
    StateEstimationThread(const StopSimulation& stop, const Map& map);

    // attach a Path observer
    void attach_state_observer(StateObserver& sobserver){sobservers_.push_back(&sobserver);}

    // update the path observers that a new path is ready
    void update_state_observers();

private:

    // run the thread
    virtual void run()override final;

    // the state of the system
    State state_;

    // the EKF filter
    //ExtendedKalmanFilter ekf_;

    /// \brief The observer list
    std::vector<StateObserver*> sobservers_;

    // Pointer to the map
    const Map* map_;
};

inline
ServerThread::StateEstimationThread::StateEstimationThread(const StopSimulation& stop, const Map& map)
    :
    StoppableTask<StopSimulation>(stop),
    state_(),
    //ekf_(),
    sobservers_(),
    map_(&map)
{
    this->set_name("StateEstimationThread");

    //create the requests and report the initial state
    state_.set(0, {"x", 0.0});
    state_.set(1, {"y", 0.0});
    state_.set(2, {"V", 0.0});
    state_.set(3, {"W", 0.0});
}


// Given the state produced by the
// StateEstimationThread produce a Path
// to be followed
struct ServerThread::PathConstructorThread: public StoppableTask<StopSimulation>
{
public:

    // The observer for the goal
    GoalObserver gobserver;

    // The observer for the state
    StateObserver sobserver;

    // constructor
    PathConstructorThread(const StopSimulation& stop, const Map& map);

    // attach a Path observer
    void attach_path_observer(PathObserver& pobserver){pobservers_.push_back(&pobserver);}

    // update the path observers that a new path is ready
    void update_path_observers();

protected:

    // run the thread
    virtual void run()override final;

    // the map used
    const Map* map_;

    // the path constructed
    Path path_;

    // the observers to notify on the path update
    std::vector<PathObserver*> pobservers_;

    // save the computed path
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

// Given the Path produced by the
// PathConstructorThread follows the Path
// by integrating the robot motion model and
// provides the appropriate corrections
struct ServerThread::PathFollowerThread: public StoppableTask<StopSimulation>
{

    // The observer for the goal
    GoalObserver gobserver;

    // The observer for the Path
    PathObserver pobserver;

    // The state observer
    StateObserver sobserver;

    // constructor
    PathFollowerThread(const StopSimulation& stop, DiffDriveVehicleWrapper& wrapper);

protected:

    // run the thread
    virtual void run()override final;

    // wrapper to the vehicle model
    DiffDriveVehicleWrapper& vwrapper_;

};

inline
ServerThread::PathFollowerThread::PathFollowerThread(const StopSimulation& stop, DiffDriveVehicleWrapper& wrapper)
    :
   StoppableTask<StopSimulation>(stop),
   gobserver(),
   pobserver(),
   sobserver(),
   vwrapper_(wrapper)
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
                           DiffDriveVehicleWrapper& vwrapper,
                           ThreadPool& thread_pool)
    :
  kernel::StoppableTask<StopSimulation>(stop_condition),
  map_(&map),
  init_state_(init_state),
  vwrapper_(vwrapper),
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

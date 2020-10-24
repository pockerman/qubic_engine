#include "exe.h"
#include "cubic_engine/search/a_star_search.h"
#include "cubic_engine/grids/waypath_builder.h"
#include "kernel/data_structs/serial_graph_builder.h"
#include "kernel/utilities/vtk_mesh_file_writer.h"
#include "kernel/maths/lp_metric.h"
#include "kernel/data_structs/boost_graph_utils.h"
#include "kernel/discretization/mesh_generation.h"
#include "kernel/utilities/csv_file_writer.h"
#include "kernel/discretization/mesh.h"
#include "kernel/discretization/element_mesh_iterator.h"
#include "kernel/discretization/mesh_predicates.h"
#include "kernel/discretization/element.h"


namespace example{

void
StateObserver::update(const StateObserver::state_resource_t& r){
    std::lock_guard<std::mutex> lock(this->mutex_);
    this->ThreadedObserverBase<std::mutex, State>::update(r);
    this->set_updated_flag(true);    
    cv.notify_all();
}

void
StateObserver::read(StateObserver::state_resource_t& r)const{

    std::unique_lock<std::mutex> lk(this->mutex_);
    cv.wait(lk, [this]{return is_updated();});
    lk.unlock();
    this->ThreadedObserverBase<std::mutex, State>::read(r);
}

void
GoalObserver::update(const GoalObserver::goal_resource_t& r){
    std::lock_guard<std::mutex> lock(this->mutex_);
    this->ThreadedObserverBase<std::mutex, Goal>::update(r);
}

void
GoalObserver::read(GoalObserver::goal_resource_t& r)const{
    std::lock_guard<std::mutex> lock(this->mutex_);
    this->ThreadedObserverBase<std::mutex, Goal>::read(r);
}

void
PathObserver::update(const PathObserver::path_resource_t& r){
    std::lock_guard<std::mutex> lock(this->mutex_);
    this->ThreadedObserverBase<std::mutex, Path*>::update(r);
    this->set_updated_flag(true);
    cv.notify_all();
}

const PathObserver::path_resource_t&
PathObserver::read()const{
    
    std::unique_lock<std::mutex> lk(this->mutex_);
    cv.wait(lk, [this]{return is_updated();});
    lk.unlock();
    return this->ThreadedObserverBase<std::mutex, Path*>::read();
}

void
RefVelocityObserver::update(const RefVelocityObserver::ref_velocity_resource_t& r){
    std::lock_guard<std::mutex> lock(this->mutex_);
    this->ThreadedObserverBase<std::mutex, real_t>::update(r);
}

void
RefVelocityObserver::read(RefVelocityObserver::ref_velocity_resource_t& r)const{
    std::lock_guard<std::mutex> lock(this->mutex_);
    this->ThreadedObserverBase<std::mutex, real_t>::read(r);
}

const DynVec 
ObservationModel::evaluate(const DynVec& input)const{

  return DynVec({input[0], input[1]});

}

const DynMat& 
ObservationModel::get_matrix(const std::string& name)const{

  if(name == "H"){
    return H_;
  }
  else if(name == "M"){
    return M_;
  }

  throw std::invalid_argument("Invalid name: "+name+" not in [H,M]");
}

void 
ObservationModel::initialize_matrices(){

  H_(0, 0) = 1.0;
  H_(1,1) = 1.0;
  M_(0,0) = 1.0;
  M_(1, 1) = 1.0; 

}

void 
StateEstimationThread::initialize(){

    std::array<real_t, 2> motion_control_error;
    motion_control_error[0] = 0.0;
    motion_control_error[1] = 0.0;

    m_model_.set_time_step(DT);
    std::map<std::string, boost::any> m_in;
    m_in["v"] = 0.0;
    m_in["w"] = 0.0;
    m_in["errors"] = motion_control_error;

    m_model_.initialize_matrices(m_in);

    {
      /// set the matrices
      DynMat P(m_model_.state_dimension, m_model_.state_dimension, 0.0);
      P(0,0) = 1.0;
      P(1,1) = 1.0;
      P(2,2) = 1.0;

      ekf_.set_matrix("P",P);

      DynMat R(2, 2, 0.0);
      R(0,0) = 1.0;
      R(1, 1) = 1.0; 

      ekf_.set_matrix("R",R);

      DynMat Q(2, 2, 0.0);
      Q(0,0) = 0.001;
      Q(1, 1) = 0.001;

      ekf_.set_matrix("Q",Q);
    }

    /// initialize the matrices for the
    /// observation model
    o_model_.initialize_matrices();
}

void
StateEstimationThread::update_state_observers(){

    const auto& computed_state = ekf_.get_state();
    state_["X"] = computed_state.get("X");
    state_["Y"] = computed_state.get("Y");
    state_["Theta"] = computed_state.get("Theta");
    state_["V"] = v_ctrl_;
    state_["W"] = w_ctrl_;

    for(uint_t o=0; o<sobservers_.size(); ++o){
        sobservers_[o]->update(state_);
    }
}

void
StateEstimationThread::set_state(const State& state){

  state_ = state;
  m_model_.set_state_property("X", state.get("X"));
  m_model_.set_state_property("Y", state.get("Y"));
  m_model_.set_state_property("Theta", state.get("Theta"));
}

void
StateEstimationThread::run(){


    std::array<real_t, 2> motion_control_error;
    motion_control_error[0] = 0.0;
    motion_control_error[1] = 0.0;

    kernel::CSVWriter writer("state.csv", ',', true);
    std::vector<std::string> names={"X","Y","Theta"};
    writer.write_column_names(names);
    
    std::vector<real_t> row(3,0.0);

    while(!goal_reached){
    
      // estimate the state
      vobserver.read(v_ctrl_);
      wobserver.read(w_ctrl_);

      std::map<std::string, boost::any> motion_input;
      motion_input["v"] = v_ctrl_;
      motion_input["w"] = w_ctrl_;
      motion_input["errors"] = motion_control_error;

      ekf_.predict(motion_input);

      auto measurement = o_model_.evaluate(ekf_.get_state().as_vector());
      ekf_.update(measurement);

      /// update the observers
      update_state_observers();

      std::lock_guard<std::mutex> lock(msg_mutex);
      std::cout<<"MESSAGE: Estimated Pos: X: "<<ekf_.get_state().get("X")
             <<", Y: "<<ekf_.get_state().get("Y")
             <<", THETA: "<<ekf_.get_state().get("Theta")
             <<", V:" <<v_ctrl_
             <<",  W:" <<w_ctrl_<<std::endl;

      row[0] = ekf_.get_state().get("X");
      row[1] = ekf_.get_state().get("Y");
      row[2] = ekf_.get_state().get("Theta");
      writer.write_row(row);

      std::this_thread::sleep_for(std::chrono::milliseconds(STATE_ESTIMATION_THREAD_CYCLE));
    }
    std::lock_guard<std::mutex> lock(msg_mutex);
    std::cout<<"MESSAGE: Task "+this->get_name()<<" exited simulation loop..."<<std::endl;      
}

void
PathConstructorThread::update_path_observers(){
    for(uint_t o =0; o<pobservers_.size(); ++o){
        pobservers_[o]->update(path_);
    }
}

void
PathConstructorThread::save_path(real_t duration){

    std::string dur = std::to_string(duration);

    std::vector<std::string> strings;
    std::istringstream f(dur);
    std::string s;
    while (std::getline(f, s, '.')) {
        strings.push_back(s);
    }

    kernel::CSVWriter writer("path.csv", ',', true);
    writer.write_mesh_nodes(path_);
}

struct Metric
{
    typedef real_t cost_t;


    template<typename Node>
    real_t operator()(const Node& s1, const Node& s2 )const{
        kernel::LpMetric<2> metric;
        return metric(s1.data.position, s2.data.position);
        //return l2Norm(s1.data.state.as_vector()-s2.data.state.as_vector());
    }
};

void
PathConstructorThread::run(){

    std::chrono::time_point<std::chrono::system_clock> start;
    std::chrono::time_point<std::chrono::system_clock> end;
    start = std::chrono::system_clock::now();

    /// the state provides the current position
    /// the goal provides the target position
    /// the goal or the state or both have not been updated wait here until
    while((!gobserver.is_updated() || !sobserver.is_updated()) && !threads_should_stop){
        std::this_thread::yield();
    }

    /// both are updated so try to establish the path
    typedef Map::vertex_t vertex_t;

    /// metric for A*
    //kernel::LpMetric<2>
    Metric heuristic;

    Goal goal;
    State state;
    Goal position;

    //Goal previous_position(-1.0);
    //Goal previous_goal;

    static auto closest_start_vertex_pred = [&](const vertex_t& vertex){
        auto distance = vertex.data.position.distance(position);

        if( distance < 1.0){
            return true;
        }
        return false;
    };

    static auto closest_goal_vertex_pred = [&](const vertex_t& vertex){
        auto distance = vertex.data.position.distance(goal);
        if( distance < 1.0){
            return true;
        }
        return false;
    };

    

        /// read both goal and state
        gobserver.read(goal);
        sobserver.read(state);

        position[0] = state.get("X");
        position[1] = state.get("Y");

        /// we need to find the starting node with the position
        /// closest to the robot position
        uint_t start_vertex_id = kernel::find_vertex(*map_, closest_start_vertex_pred);
        uint_t goal_vertex_id = kernel::find_vertex(*map_, closest_goal_vertex_pred);

        /// if this is an invalid id then throw?
        if(start_vertex_id == kernel::KernelConsts::invalid_size_type() ||
            goal_vertex_id == kernel::KernelConsts::invalid_size_type()    ){

#ifdef USE_LOG
        kernel::Logger::log_info("Using starting vertex: "+std::to_string(start_vertex_id));
        kernel::Logger::log_info("Using ending vertex: "+std::to_string(goal_vertex_id));
#endif
            throw std::logic_error("Invalid vertex id for path finding");
        }

        vertex_t start_pos;
        start_pos.data.position = position;
        start_pos.id = start_vertex_id;

        vertex_t goal_pos;
        goal_pos.data.position = goal;
        goal_pos.id = goal_vertex_id;

        /// find the path we need the goal
        auto path_connections = cengine::astar_search(const_cast<Map&>(*map_), start_pos, goal_pos, heuristic );

        if(path_connections.empty()){

#ifdef USE_LOG
        kernel::Logger::log_error("Path connections are empty");
#endif
            throw std::logic_error("Path connections are empty");
        }
#ifdef USE_LOG
        else{
            kernel::Logger::log_info("There are: "+std::to_string(path_connections.size())+" path connections");
        }
#endif

        /// update the line mesh that represents the path
        std::vector<uint_t> mesh_data = cengine::reconstruct_a_star_path(path_connections, goal_vertex_id);

#ifdef USE_LOG
        kernel::Logger::log_info("Reconstructed path from A*");
#endif

        /// update the path
        cengine::grids::build(path_, *map_, mesh_data);

#ifdef USE_LOG
        kernel::Logger::log_info("Updated Path");
#endif

        /// update the observers
        update_path_observers();

        end = std::chrono::system_clock::now();
        std::chrono::duration<real_t> duration = end - start;

        /// save the path
        save_path(duration.count());       
}

void
PathFollowerThread::run(){

    /// we should wait until we have a path
    /// PathConstructorThread is one off so
    /// reading it constantly will cause a deadlock
    path_ = &pobserver.read();

    kernel::CSVWriter writer("path_follower.csv", ',', true);
    std::vector<real_t> row(2, 0.0);

    while(!this->should_stop() && !threads_should_stop){

        Goal position({state_.get("X"), state_.get("Y")});

        /// the goal is updated at the
        /// beginning of the program
        Goal goal;
        gobserver.read(goal);

        if(goal.distance(position) < gradius_){

            /// the goal has been reached
            goal_reached = true;
            threads_should_stop = true;
            this->get_condition().set_condition(true);
#ifdef USE_LOG
        kernel::Logger::log_info("GOAL: " + goal.to_string() + " REACHED WITH RADIOUS: "+std::to_string(gradius_));
#endif
          break;
        }
        else{

            SysState<3> real_state(state_);

            /// update the path
            path_controller_.update(*path_);
            
            /// calculate the steering CMD
            auto [control_result, lookahed_point, closest] = path_controller_.execute(real_state);

            /// update the observers interested
            update_w_velocity_observers(control_result);

            row[0] = lookahed_point[0];
            row[1] = lookahed_point[1];
            writer.write_row(row);
        }

        ///... sleep for some time
        std::this_thread::sleep_for(std::chrono::milliseconds(PATH_CORRECTION_THREAD_CYCLE));
        
        if(sobserver.is_updated()){
          sobserver.read(state_);
        }
    }

    std::lock_guard<std::mutex> lock(msg_mutex);
    std::cout<<"MESSAGE: Task "+this->get_name()<<" exited simulation loop..."<<std::endl;
}
}


int main(){

#ifdef USE_LOG
    kernel::Logger::set_log_file_name("log_file.log");
#endif

    using namespace example;

    
    typedef StateEstimationThread state_est_task_t;
    typedef PathConstructorThread path_cstr_task_t;
    typedef PathFollowerThread path_follow_task_t;

    /// create the graph from the mesh
    Map map;

    {
        uint_t nx = 10;
        uint_t ny = 10;

        GeomPoint<2> start(0.0);
        GeomPoint<2> end(10.0);

        Mesh<2> mesh;

        // generate the mesh
        kernel::numerics::build_quad_mesh(mesh, nx, ny, start, end);

        kernel::numerics::VtkMeshFileWriter writer("map.vtk", true);
        std::vector<real_t> occupamcy_prob(mesh.n_elements(), 0.0);

        occupamcy_prob[4] = 1.0;
        occupamcy_prob[8] = 1.0;
        occupamcy_prob[9] = 1.0;
        occupamcy_prob[18] = 1.0;
        occupamcy_prob[19] = 1.0;
        occupamcy_prob[14] = 1.0;
        occupamcy_prob[24] = 1.0;
        occupamcy_prob[11] = 1.0;
        occupamcy_prob[12] = 1.0;
        occupamcy_prob[21] = 1.0;
        occupamcy_prob[22] = 1.0;

        occupamcy_prob[42] = 1.0;
        occupamcy_prob[43] = 1.0;
        occupamcy_prob[52] = 1.0;
        occupamcy_prob[53] = 1.0;

        occupamcy_prob[36] = 1.0;
        occupamcy_prob[37] = 1.0;
        occupamcy_prob[46] = 1.0;
        occupamcy_prob[47] = 1.0;

        occupamcy_prob[65] = 1.0;
        occupamcy_prob[66] = 1.0;
        occupamcy_prob[75] = 1.0;
        occupamcy_prob[76] = 1.0;

        occupamcy_prob[91] = 1.0;
        occupamcy_prob[92] = 1.0;
        occupamcy_prob[93] = 1.0;
        occupamcy_prob[94] = 1.0;

        occupamcy_prob[81] = 1.0;
        occupamcy_prob[82] = 1.0;
        occupamcy_prob[83] = 1.0;
        occupamcy_prob[84] = 1.0;

        writer.write_mesh(mesh, occupamcy_prob, "OccupancyProb");

        /// build the mesh graph
        kernel::build_mesh_graph(mesh, map);

        for(uint_t v=0; v<map.n_vertices(); ++v){
           auto& vertex = map.get_vertex(v);

           uint_t id = vertex.id;
           auto* element = mesh.element(id);
           vertex.data.position = element->centroid();
           vertex.data.occumancy_prob = occupamcy_prob[id];
        }
    }

    State init_state;
    init_state.set(0, {"X",0.5});
    init_state.set(1, {"Y",0.5});
    init_state.set(2, {"Theta",0.0});
    
    /// input for the path follower
    CarrotChasingPathTrackControllerInput path_control_input;
    path_control_input.lookahead_distance = 0.4;
    path_control_input.n_sampling_points = 10;
    path_control_input.k = 1.0;

    real_t gradius = 0.2;

    ThreadPoolOptions options;
    options.n_threads = 2;
    options.msg_on_start_up = true;
    options.msg_on_shut_down = true;
    options.msg_when_adding_tasks = true;
    options.start_on_construction = true;

    // executor
    ThreadPool pool(options);

    StopSimulation stop_sim;
    std::vector<std::unique_ptr<kernel::TaskBase>> tasks;
    tasks.reserve(2);

    tasks.push_back(std::make_unique<path_cstr_task_t>(stop_sim, map));
    tasks.push_back(std::make_unique<path_follow_task_t>(stop_sim, path_control_input, gradius ));

    /// the state estimation
    StateEstimationThread state_est_task(stop_sim, map);

    bool exit = false;

    while(!exit){

      std::vector<real_t> values = {0.0, 0.0};
      std::cout<<RESPONSE<<"Enter new goal: "<<std::endl;
      
      values[0] = 9.5;
      values[1] = 9.5;
     
      std::cout<<"Goal assigned: "<<values[0]<<","<<values[1]<<std::endl;
      Goal goal(values);

      real_t V = 0.5; 
      init_state.set(3, {"V",V});
    
      real_t W = 0.0;
      init_state.set(4, {"W",W});
      
      /// update the goal observers
      static_cast<path_cstr_task_t*>(tasks[0].get())->gobserver.update(goal);
      static_cast<path_follow_task_t*>(tasks[1].get())->gobserver.update(goal);

      /// add the path observers that the PathConstructorThread
      /// should update
      static_cast<path_cstr_task_t*>(tasks[0].get())->attach_path_observer(static_cast<path_follow_task_t*>(tasks[1].get())->pobserver);
    
     
      /// the state estimation task must know W
      static_cast<path_follow_task_t*>(tasks[1].get())->attach_w_velocity_observer(state_est_task.wobserver);
      static_cast<path_follow_task_t*>(tasks[1].get())->set_state(init_state);
    
      state_est_task.vobserver.update(V);
      state_est_task.wobserver.update(W);
      state_est_task.initialize();
      state_est_task.set_state(init_state);

      state_est_task.attach_state_observer(static_cast<path_cstr_task_t*>(tasks[0].get())->sobserver);        
      state_est_task.attach_state_observer(static_cast<path_follow_task_t*>(tasks[1].get())->sobserver);

      // add the tasks... 
      pool.add_tasks(tasks);

      /// run the task on the main thread
      /// this should run until we meet
      /// the goal
      state_est_task.run();
     
    
      {
        std::lock_guard<std::mutex> lock(msg_mutex);
        std::cout<<MESSAGE<<"GOAL "+goal.to_string()<<" REACHED WITH RADIOUS " + std::to_string(gradius)<<std::endl;
        std::string answer;
        
        std::cout<<RESPONSE<<"Exit simulation: (y/n)"<<std::endl;
        std::cin>>answer;

        if(answer == "y"){
          exit=true;
        }
      }

      threads_should_stop = true;
  }
    
    pool.close();
    return 0;
}

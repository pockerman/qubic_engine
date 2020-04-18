#include "exe.h"
#include "cubic_engine/search/a_star_search.h"
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
}

void
StateObserver::read(StateObserver::state_resource_t& r)const{
    std::lock_guard<std::mutex> lock(this->mutex_);
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
}

const PathObserver::path_resource_t&
PathObserver::read()const{
    std::lock_guard<std::mutex> lock(this->mutex_);
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

void
MeasurmentObserver::update(const MeasurmentObserver::measurement_resource_t& r){
    std::lock_guard<std::mutex> lock(this->mutex_);
    this->ThreadedObserverBase<std::mutex, DynVec>::update(r);
}

void
MeasurmentObserver::read(MeasurmentObserver::measurement_resource_t& r)const{
    std::lock_guard<std::mutex> lock(this->mutex_);
    this->ThreadedObserverBase<std::mutex, DynVec>::read(r);
}

void
ServerThread::StateEstimationThread::update_state_observers(){

    for(uint_t o=0; o<sobservers_.size(); ++o){
        sobservers_[o]->update(state_);
    }
}

void
ServerThread::StateEstimationThread::run(){

    std::array<real_t, 2> motion_control_error;
    motion_control_error[0] = 0.0;
    motion_control_error[1] = 0.0;
    while(!this->should_stop()){

        // estimate the state
        real_t v=0.0;
        vobserver.read(v);

        real_t w=0.0;
        wobserver.read(w);

        auto motion_input = std::make_tuple(v*DT, w*DT, motion_control_error);
        ekf_.predict(motion_input);

        /// Read the measurement vector
        typedef MeasurmentObserver::measurement_resource_t measurement_t;
        measurement_t measurement;
        mobserver.read(measurement);

        ekf_.update(measurement);

        // update the observers
        //update_state_observers();

        std::this_thread::sleep_for(std::chrono::milliseconds(STATE_ESTIMATION_THREAD_CYCLE));
    }

    std::lock_guard<std::mutex> lock(msg_mutex);
    std::cout<<"MESSAGE: Task "+this->get_name()<<" exited simulation loop..."<<std::endl;
}

void
ServerThread::PathConstructorThread::update_path_observers(){
    for(uint_t o =0; o<pobservers_.size(); ++o){
        pobservers_[o]->update(path_);
    }
}

void
ServerThread::PathConstructorThread::save_path(real_t duration){

    std::string dur = std::to_string(duration);

    std::vector<std::string> strings;
    std::istringstream f(dur);
    std::string s;
    while (std::getline(f, s, '.')) {
        strings.push_back(s);
    }

    kernel::CSVWriter writer("path_" + strings[1] + ".csv", ',', true);
    writer.write_mesh(path_);
}

void
ServerThread::PathConstructorThread::run(){

    std::chrono::time_point<std::chrono::system_clock> start;
    std::chrono::time_point<std::chrono::system_clock> end;
    start = std::chrono::system_clock::now();

    // the state provides the current position
    // the goal provides the target position
    // the goal or the state or both have not been updated wait here until
    while((!gobserver.is_updated() || !sobserver.is_updated()) && !threads_should_stop){
        std::this_thread::yield();
    }

    /// both are updated so try to establish the path
    typedef Map::vertex_type vertex_t;

    /// metric for A*
    kernel::LpMetric<2> h;

    Goal goal;
    State state;
    Goal position;

    Goal previous_position(-1.0);
    Goal previous_goal;

    static auto closest_start_vertex_pred = [&](const vertex_t& vertex){
        auto distance = vertex.data.position.distance(position);

        if( distance < 0.1){
            return true;
        }
        return false;
    };

    static auto closest_goal_vertex_pred = [&](const vertex_t& vertex){
        auto distance = vertex.data.position.distance(goal);
        if( distance < 0.1){
            return true;
        }
        return false;
    };

    while(!this->should_stop()){

        // read both goal and state
        gobserver.read(goal);
        sobserver.read(state);

        position[0] = state.get("X");
        position[1] = state.get("Y");

        // if position has not changed
        // then the robot for some reason does not move
        // maybe it is waiting for a new goal
        // if the position is the same and the goal
        // does not change then we don't need to do anything
        if(previous_goal.distance(goal) < 1.0e-3 &&
           previous_position.distance(position) < 1.0e-3){
#ifdef USE_LOG
            kernel::Logger::log_warning("Previous goal is the same with current goal");
            kernel::Logger::log_warning("Previous state is the same with state goal");
            kernel::Logger::log_warning("Skipping computation...");
#endif
            continue;
        }

        // we need to find the starting node with the position
        // closest to the robot position
        uint_t start_vertex_id = kernel::find_vertex(*map_, closest_start_vertex_pred);
        uint_t goal_vertex_id = kernel::find_vertex(*map_, closest_goal_vertex_pred);

        // if this is an invalid id then throw?
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

        // find the path we need the goal
        auto path_connections = cengine::astar_search(const_cast<Map&>(*map_), start_pos, goal_pos, h );

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

        // update the line mesh that represents the path
        std::vector<uint_t> mesh_data = cengine::reconstruct_a_star_path(path_connections, goal_vertex_id);

#ifdef USE_LOG
        kernel::Logger::log_info("Reconstructed path from A*");
#endif

        // now that we have the connections let's update the path
        kernel::numerics::build_mesh(path_, *map_, mesh_data);

#ifdef USE_LOG
        kernel::Logger::log_info("Updated Path");
#endif

        // update the observers
        update_path_observers();

        end = std::chrono::system_clock::now();
        std::chrono::duration<real_t> duration = end - start;

        // save the path
        save_path(duration.count());

        previous_position = position;
        previous_goal = goal;

        // sleep for some time
        std::this_thread::sleep_for(std::chrono::milliseconds(PATH_CONSTRUCTOR_CYCLE));
    }

    std::lock_guard<std::mutex> lock(msg_mutex);
    std::cout<<"MESSAGE: Task "+this->get_name()<<" exited simulation loop..."<<std::endl;
}

void
ServerThread::PathFollowerThread::run(){

    while(!this->should_stop()){


        // are we on the goal? if yes
        // then notify server that notfies the client
        // and sleep until a new mission arrives
        // or exit has been signaled

        // if we are not on the goal check
        // are we on path? if yes continue
        // else make the corrections

    }

    std::lock_guard<std::mutex> lock(msg_mutex);
    std::cout<<"MESSAGE: Task "+this->get_name()<<" exited simulation loop..."<<std::endl;
}

void
ServerThread::RequestTask::update_goal_observers(const Goal& goal){

    /// update self
    gobserver.update(goal);

    /// loop over the observers and update them
    for(uint_t o=0; o<goal_observers_.size(); ++o){
        goal_observers_[o]->update(goal);
    }
}

void
ServerThread::RequestTask::update_v_observers(real_t v){

    vobserver.update(v);

    // loop over the observers and update them
    for(uint_t o=0; o<v_observers_.size(); ++o){
        v_observers_[o]->update(v);
    }
}

void
ServerThread::RequestTask::update_w_observers(real_t w){

    wobserver.update(w);

    // loop over the observers and update them
    for(uint_t o=0; o<w_observers_.size(); ++o){
        w_observers_[o]->update(w);
    }
}

void
ServerThread::RequestTask::run(){

    // the object that handles the solution output
    kernel::CSVWriter writer("vehicle_state.csv");

    while(!this->should_stop()){

        save_solution(writer);

        // otherwise create the input
        while(!requests_.empty()){
            auto request = requests_.pop_wait();
            serve_request(request);
        }

        // if the request is not empty put to sleep
        std::this_thread::sleep_for(std::chrono::milliseconds(REQUESTS_THREAD_CYCLE));
    }

    std::lock_guard<std::mutex> lock(msg_mutex);
    std::cout<<"MESSAGE: Task "+this->get_name()<<" exited simulation loop..."<<std::endl;
}

void
ServerThread::RequestTask::serve_request(const std::string& request){

    std::vector<std::string> strings;
    std::istringstream f(request);
    std::string s;
    while (std::getline(f, s, ';')) {
        strings.push_back(s);
    }

    if(!strings.empty()){
        if(strings[0] == "CMD"){

            responses_.push_item("CMDs are not allowed yet");

            //we have a CMD
            /*if(strings[1] == "S"){
                auto [x, y, theta, velocity]  = vwrapper_.get_state();
                cmds_.push_item(CMD::generate_cmd(0.0, theta, strings[1]));
            }
            else if(strings[1] == "V"){

                    auto vel_str = strings[2];
                    real_t vel = std::atof(vel_str.c_str());

                    auto [x, y, theta, velocity]  = vwrapper_.get_state();
                    real_t w = theta;

                    if(strings.size() == 5 && strings[3] == "W" ){

                        auto w_str = strings[4];
                        w = std::atof(w_str.c_str());
                    }

                    cmds_.push_item(CMD::generate_cmd(vel*DT, w*DT, strings[1]));
            }
            else if(strings[1] == "W"){

                    auto w_str = strings[2];
                    real_t w = std::atof(w_str.c_str());

                    auto [x, y, theta, velocity]  = vwrapper_.get_state();
                    real_t v = velocity;

                    if(strings.size() == 5 && strings[3] == "V" ){

                        auto v_str = strings[4];
                        v = std::atof(v_str.c_str());
                    }

                    cmds_.push_item(CMD::generate_cmd(v*DT, w*DT, strings[1]));

            }*/
        }
        else if(strings[0] == SET_GOAL_SERVER){
            responses_.push_item(SET_GOAL);
        }
        else if(strings[0] == GOAL){
            kernel::Logger::log_info("Received GOAL request");

            if(strings.size() != 3){
                responses_.push_item("You want to update the goal. 2 coordinates are required separated by ;.");
                responses_.push_item(SET_GOAL);
            }
            else{
                // this is a new goal
                auto x = std::atof(strings[1].c_str());
                auto y = std::atof(strings[2].c_str());

                std::vector<real_t> coords(2);
                coords[0] = x;
                coords[1] = y;
                Goal g(coords);
                kernel::Logger::log_info("Update goal: " + g.to_string());
                update_goal_observers(g);
            }
        }
        else if(strings[0] == V_CMD){

            kernel::Logger::log_info("Update ref velocity: " + strings[1]);
            auto v = std::atof(strings[1].c_str());
            update_v_observers(v);
        }
        else if(strings[0] == W_CMD){
             kernel::Logger::log_info("Update ref angular velocity: " + strings[1]);
            auto w = std::atof(strings[1].c_str());
            update_w_observers(w);
        }
        else if(strings[0] == EXIT){
            kernel::Logger::log_info("Received EXIT request");

            // notify the others that we stop
            threads_should_stop = true;
            this->get_condition().set_condition(true);   
        }
        else if(strings[0] == PRINT){
            kernel::Logger::log_info("Received PRINT request");
            State s;
            sobserver.read(s);
            kernel::Logger::log_info("Sendig PRINT request " + s.as_string());
            responses_.push_item(s.as_string());
        }
        else{
           responses_.push_item("You entered an unknown command. Try again");
        }
    }
}

void
ServerThread::RequestTask::save_solution(kernel::CSVWriter& writer){

    //static State old;
    //static State current;
    //sobserver.read(current);

    /*std::vector<real_t> row(2);
    GeomPoint<2> current(std::vector<real_t>({x,y}));

    if(position_.distance(current) > 100.0){
        writer.open(std::ios_base::app);

        row[0] = x;
        row[1] = y;
        writer.write_row(row);
        writer.close();
        position_ = current;
    }*/
}

void
ServerThread::ClientTask::run(){

    /// wait for a while until
    /// the server responses are ready
    /// the user should set the goal position, the velocity
    /// and the angular velocity of the robot
    while(responses_.empty() || responses_.size() != 3){
        std::this_thread::yield();
    }

    std::string request("");

    /// on start up we want to catch what
    /// is coming from the server
    while(!responses_.empty()){

        //empty what is comming from the server
        auto response = responses_.pop_wait();

        if(response == SET_GOAL){

            // then wait until we receive the new goal
            std::vector<real_t> values = {0.0, 0.0};

            std::cout<<RESPONSE<<"Enter new goal: "<<std::endl;
            std::cout<<"Enter first coordinate of goal"<<std::endl;
            std::cin>>values[0];
            std::cout<<"Enter second coordinate of goal"<<std::endl;
            std::cin>>values[1];

            std::cout<<"Goal assigned: "<<values[0]<<","<<values[1]<<std::endl;
            request = "GOAL;" + std::to_string(values[0]) + ";"+std::to_string(values[1]);

            requests_.push_item(request);
        }
        else if(response == V_CMD ){
            real_t velocity = 0.0;
            std::cout<<RESPONSE<<"Enter robot velocity: "<<std::endl;
            std::cin>>velocity;
            request = V_CMD + ";" + std::to_string(velocity);
            requests_.push_item(request);
        }
        else if(response == W_CMD ){
            real_t velocity = 0.0;
            std::cout<<RESPONSE<<"Enter robot angular velocity: "<<std::endl;
            std::cin>>velocity;
            request = W_CMD + ";" + std::to_string(velocity);
            requests_.push_item(request);
        }
        else{
            std::cout<<RESPONSE<<response<<std::endl;
        }
    }

    request = ENTER_CMD;
    while(!this->should_stop()){

        if(request == ENTER_CMD){

            std::cout<<"MESSAGE: Enter CMD for robot..."<<std::endl;

            // Read the CMD
            std::getline(std::cin, request);
        }
        else if(request == EXIT){
            std::cout<<"MESSAGE: CMD requested: "<<request<<std::endl;
            this->get_condition().set_condition(true);
            requests_.push_item(request);
            break;
        }
        else if(request == EMPTY_CMD){
            request = ENTER_CMD;
        }
        else if(request == PRINT){
           std::cout<<"MESSAGE: CMD requested: "<<PRINT<<std::endl;
           requests_.push_item(request);

           auto response = responses_.pop_wait();
           std::cout<<RESPONSE<<response<<std::endl;
           request = ENTER_CMD;
        }
        else if(request == PRINT_GOAL){
            std::cout<<"MESSAGE: CMD requested: "<<PRINT_GOAL<<std::endl;
            requests_.push_item(request);
            auto response = responses_.pop_wait();
            std::cout<<RESPONSE<<response<<std::endl;
            request = ENTER_CMD;
        }
        else if(request == SET_GOAL){

            std::cout<<"MESSAGE: CMD requested: "<<SET_GOAL<<std::endl;
            // then wait until we receive the new goal
            std::vector<real_t> values = {0.0, 0.0};

            std::cout<<RESPONSE<<"Enter new goal: "<<std::endl;
            std::cout<<"Enter first coordinate of goal"<<std::endl;
            std::cin>>values[0];
            std::cout<<"Enter second coordinate of goal"<<std::endl;
            std::cin>>values[1];
            std::cout<<"Goal assigned: "<<values[0]<<","<<values[1]<<std::endl;

            request = "GOAL;" + std::to_string(values[0]) + ";"+std::to_string(values[1]);
            requests_.push_item(request);
            request = ENTER_CMD;

        }
        else{
            std::cout<<"You entered an unknown CMD. Try again"<<std::endl;
            request = ENTER_CMD;
        }
    }

    std::lock_guard<std::mutex> lock(msg_mutex);
    std::cout<<"MESSAGE: Task "+this->get_name()<<" exited simulation loop..."<<std::endl;
}

void
ServerThread::run(){

#ifdef USE_LOG
    kernel::Logger::log_info("Starting Server Thread");
#endif

    // create the tasks and assign them
    // to the queue
    tasks_.reserve(5);

    typedef ServerThread::RequestTask request_task_t;
    typedef ServerThread::ClientTask client_task_t;
    typedef ServerThread::StateEstimationThread state_est_task_t;
    typedef ServerThread::PathConstructorThread path_cstr_task_t;
    typedef ServerThread::PathFollowerThread path_follow_task_t;

    tasks_.push_back(std::make_unique<state_est_task_t>(this->get_condition(), *map_));
    tasks_.push_back(std::make_unique<path_cstr_task_t>(this->get_condition(), *map_));
    tasks_.push_back(std::make_unique<path_follow_task_t>(this->get_condition()));

    tasks_.push_back(std::make_unique<request_task_t>(this->get_condition(), requests_, cmds_, responses_));
    tasks_.push_back(std::make_unique<client_task_t>(this->get_condition(), requests_, responses_));

    //assign the observers
    {
        request_task_t* req_task = static_cast<request_task_t*>(tasks_[3].get());
        req_task->attach_goal_observer(static_cast< path_cstr_task_t*>(tasks_[1].get())->gobserver);
        req_task->attach_goal_observer(static_cast<path_follow_task_t*>(tasks_[2].get())->gobserver);
        req_task->attach_v_observer(static_cast<state_est_task_t*>(tasks_[0].get())->vobserver);
        req_task->attach_w_observer(static_cast<state_est_task_t*>(tasks_[0].get())->wobserver);

        state_est_task_t* state_thread = static_cast<state_est_task_t*>(tasks_[0].get());
        state_thread->attach_state_observer(static_cast<path_cstr_task_t*>(tasks_[1].get())->sobserver);
        state_thread->attach_state_observer(static_cast<path_follow_task_t*>(tasks_[2].get())->sobserver);
        state_thread->attach_state_observer(static_cast<request_task_t*>(tasks_[3].get())->sobserver);

        //ask the state thread to update the state observers
        state_thread->update_state_observers();

        path_cstr_task_t* pconstruct_thread = static_cast< path_cstr_task_t*>(tasks_[1].get());
        pconstruct_thread->attach_path_observer(static_cast<path_follow_task_t*>(tasks_[2].get())->pobserver);
        pconstruct_thread->attach_path_observer(static_cast<request_task_t*>(tasks_[3].get())->pobserver);

        this->attach_measurement_observer(state_thread->mobserver);

    }

    // create a request so that we print the state
    // when the ClientTask starts
    requests_.push_item(PRINT);

    /// we also need to add a response so that the
    /// ClientTask asks  the user to set the goal
    /// enters the goal
    requests_.push_item(SET_GOAL_SERVER);

    /// set velocity request to start the simulation
    requests_.push_item(V_CMD);

    /// set angular velocity request to start the simulation
    requests_.push_item(W_CMD);

    /// add the tasks this is where tasks get executed
    thread_pool_.add_tasks(tasks_);

    while(!this->should_stop()){

        auto measurement = get_measurement();
        update_measurement_observers(measurement);
        tasks_stopped();
    }

    std::lock_guard<std::mutex> lock(msg_mutex);
    std::cout<<"MESSAGE: Task "+this->get_name()<<" exited simulation loop..."<<std::endl;

#ifdef USE_LOG
    kernel::Logger::log_info("Stopping Server Thread");
#endif
}

void
ServerThread::update_measurement_observers(MeasurmentObserver::measurement_resource_t& measurement){
    for(uint_t o=0; o<m_observers_.size(); ++o){
        m_observers_[o]->update(measurement);
    }
}

const DynVec
ServerThread::get_measurement()const{
    return DynVec();
}

void
ServerThread::tasks_stopped(){

    if(threads_should_stop){

        this->get_condition().set_condition(true);

        for(uint_t t=0; t<tasks_.size(); ++t){
           dynamic_cast<StoppableTask<StopSimulation>*>(tasks_[t].get())->get_condition().set_condition(true);
        }
    }
}

}


int main(){

#ifdef USE_LOG
    kernel::Logger::set_log_file_name("log_file.log");
#endif

    using namespace example;

    // create the graph from the mesh
    Map map;

    {
        uint_t nx = 10;
        uint_t ny = 10;

        GeomPoint<2> start(0.0);
        GeomPoint<2> end(1.0);

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

        // build the mesh graph
        kernel::build_mesh_graph(mesh, map);

        for(uint_t v=0; v<map.n_vertices(); ++v){
           auto& vertex = map.get_vertex(v);

           uint_t id = vertex.id;
           auto* element = mesh.element(id);
           vertex.data.position = element->centroid();
           vertex.data.occumancy_prob = occupamcy_prob[id];
        }

    }

    const uint_t N_THREADS = 5;
    const real_t RADIUS = 2.0/100.0;

    // the object that controls when to stop
    // the threads
    StopSimulation stop_sim;

    // the initial system state
    SysState<4> init_state;

    DiffDriveProperties properties;
    properties.R = RADIUS;

    // the vehicle the simulator is simulating
    DiffDriveVehicle vehicle(properties);

    ThreadPoolOptions options;
    options.n_threads = N_THREADS;
    options.msg_on_start_up = true;
    options.msg_on_shut_down = true;
    options.msg_when_adding_tasks = true;
    options.start_on_construction = true;

    // executor
    ThreadPool pool(options);

    // the server instance
    ServerThread server(stop_sim, map, init_state, pool);

    server.run();
    pool.close();
    return 0;
}

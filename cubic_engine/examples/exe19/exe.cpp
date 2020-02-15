#include "exe.h"

#include "cubic_engine/search/a_star_search.h"
#include "kernel/data_structs/serial_graph_builder.h"
#include "kernel/utilities/vtk_mesh_file_writer.h"
#include "cubic_engine/search/a_star_search.h"
#include "kernel/maths/lp_metric.h"


namespace example
{

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

void
PathObserver::read(PathObserver::path_resource_t& r)const{
    std::lock_guard<std::mutex> lock(this->mutex_);
    this->ThreadedObserverBase<std::mutex, Path*>::read(r);
}

DiffDriveVehicleWrapper::DiffDriveVehicleWrapper(DiffDriveVehicle& vehicle)
    :
   integrate_mutex_(),
   vehicle_ptr_(&vehicle),
   cmd_()
{}

std::string
DiffDriveVehicleWrapper::integrate(){
    std::lock_guard<std::mutex> lock(integrate_mutex_);

    if(cmd_.name != "INVALID_NAME"){
        vehicle_ptr_->integrate(cmd_.velocity, cmd_.orientation);
    }

    return cmd_.name;
}

std::string
DiffDriveVehicleWrapper::get_state_as_string()const{

    std::lock_guard<std::mutex> lock(integrate_mutex_);

    auto x = vehicle_ptr_->get_x_position();
    auto y = vehicle_ptr_->get_y_position();
    auto theta = vehicle_ptr_->get_orientation();
    auto velocity = vehicle_ptr_->get_velcoty();
    return std::to_string(x)+","+std::to_string(y)+","+std::to_string(theta)+","+std::to_string(velocity)+"\n";

}

std::tuple<real_t, real_t>
DiffDriveVehicleWrapper::get_position()const{

    std::lock_guard<std::mutex> lock(integrate_mutex_);

    auto x = vehicle_ptr_->get_x_position();
    auto y = vehicle_ptr_->get_y_position();
    return {x,y};
}

std::tuple<real_t, real_t, real_t, real_t>
DiffDriveVehicleWrapper::get_state()const{

    auto x = vehicle_ptr_->get_x_position();
    auto y = vehicle_ptr_->get_y_position();
    auto theta = vehicle_ptr_->get_orientation();
    auto velocity = vehicle_ptr_->get_velcoty();
    return {x, y, theta, velocity};

}

void
DiffDriveVehicleWrapper::set_cmd(CMD cmd){
    std::lock_guard<std::mutex> lock(integrate_mutex_);
    cmd_ = cmd;
}


void
ServerThread::StateEstimationThread::update_state_observers(){

    for(uint_t o=0; o<sobservers_.size(); ++o){
        sobservers_[o]->update(state_);
    }
}

void
ServerThread::StateEstimationThread::run(){

    while(!this->should_stop()){

        // estimate the state

        //SysState<4> state;

        // update the observers only if the
        // we are at the update rate
        //update_state_observers();
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
ServerThread::PathConstructorThread::run(){

    // the goal or the state
    // or both has not been updated wait here until
    // we get new info
    /*while(!gobserver.is_updated() || !sobserver.is_updated()){
        std::this_thread::yield();
    }*/

    typedef Map::vertex_type vertex_t;
    std::vector<real_t> pos(2);
    kernel::LpMetric<2> h;

    while(!this->should_stop()){

        /*Goal goal;
        gobserver.read(goal);

        State state;
        sobserver.read(state);

        pos[0] = state.get("y");
        pos[1] = state.get("y");
        GeomPoint<2> current(pos);

        vertex_t start_pos;
        start_pos.data.position = GeomPoint<2>(pos);
        vertex_t goal_pos;
        goal_pos.data.position = goal;


        // find the path we need the goal
        auto path_connections = cengine::astar_search(const_cast<Map&>(*map_), start_pos, goal_pos, h );

        // update the line mesh that represents the path
        //std::vector<uint_t> mesh_data = cengine::reconstruct_a_star_path(path_connections, start_node_id);

        // update the observers
        update_path_observers();*/
    }


    std::lock_guard<std::mutex> lock(msg_mutex);
    std::cout<<"MESSAGE: Task "+this->get_name()<<" exited simulation loop..."<<std::endl;
}

void
ServerThread::PathFollowerThread::run(){

    while(!this->should_stop()){

        vwrapper_.integrate();

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

    gobserver.update(goal);

    // loop over the observers and update them
    for(uint_t o=0; o<goal_observers_.size(); ++o){
        goal_observers_[o]->update(goal);
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

            // if the request is a CMD
            serve_request(request);
        }
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

            //std::lock_guard<std::mutex> lock(msg_mutex);
            //std::cout<<"Received GOAL request"<<std::endl;
            kernel::Logger::log_info("Received GOAL request");
            //std::cout<<"Logged..."<<std::endl;

            if(strings.size() != 3){
                responses_.push_item("You want to update the goal. 2 coordinates are required separated by ;.");
                responses_.push_item(SET_GOAL);
            }
            else{
                // this is a new goal
                //auto x = std::atof(strings[1].c_str());
                //auto y = std::atof(strings[2].c_str());

                //std::vector<real_t> coords(2);
                //coords[0] = x;
                //coords[1] = y;
                //Goal g(coords);
                kernel::Logger::log_info("Update coordinates");
                //update_goal_observers(g);
            }
        }
        else if(strings[0] == EXIT){

            this->get_condition().set_condition(true);
            //CMD exit(0.0, 0.0, "EXIT");
            //cmds_.push_item(exit);
            threads_should_stop = true;
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

void
ServerThread::ClientTask::run(){



    // wait for a while until
    // the server responses are ready
    while(responses_.empty()){
        std::this_thread::yield();
    }

    std::string request("");

    // on start up we want to catch what
    // is coming from the server
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
        else{
            std::cout<<RESPONSE<<response<<std::endl;
        }
    }

    while(!this->should_stop()){

        request = "";
        std::cout<<"MESSAGE: Enter CMD for robot..."<<std::endl;

        // Read the CMD
        std::getline(std::cin, request);

        //std::cout<<"MESSAGE: CMD entered..."<<request<<std::endl;

        if(request == EXIT){

            std::cout<<"MESSAGE: CMD requested: "<<request<<std::endl;
            this->get_condition().set_condition(true);
            requests_.push_item(request);
            break;
        }
        else if(request == EMPTY_CMD){
            std::cout<<"MESSAGE: CMD requested: EMPTY_CMD"<<std::endl;
            std::cout<<EMPTY_CMD<<std::endl;
        }
        else if(request == PRINT){
           std::cout<<"MESSAGE: CMD requested: "<<PRINT<<std::endl;
           requests_.push_item(request);
        }
        else if(request == PRINT_GOAL){
            std::cout<<"MESSAGE: CMD requested: "<<PRINT_GOAL<<std::endl;
            requests_.push_item(request);
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

        }
        else{
            std::cout<<"You entered an unknown CMD. Try again"<<std::endl;
            //requests_.push_item(request);
        }

        while(!responses_.empty()){
            //empty what is comming from the server
            auto response = responses_.pop_wait();
            std::cout<<RESPONSE<<response<<std::endl;
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
    tasks_.push_back(std::make_unique<path_follow_task_t>(this->get_condition(), vwrapper_));


    tasks_.push_back(std::make_unique<request_task_t>(this->get_condition(), requests_, cmds_, responses_));
    tasks_.push_back(std::make_unique<client_task_t>(this->get_condition(), requests_, responses_));

    //assign the observers
    {
//        request_task_t* req_task = static_cast<request_task_t*>(tasks_[3].get());
//        req_task->attach_goal_observer(static_cast< path_cstr_task_t*>(tasks_[1].get())->gobserver);
//        req_task->attach_goal_observer(static_cast<path_follow_task_t*>(tasks_[4].get())->gobserver);

        state_est_task_t* state_thread = static_cast<state_est_task_t*>(tasks_[0].get());
        state_thread->attach_state_observer(static_cast<path_cstr_task_t*>(tasks_[1].get())->sobserver);
        state_thread->attach_state_observer(static_cast<path_follow_task_t*>(tasks_[2].get())->sobserver);
        state_thread->attach_state_observer(static_cast<request_task_t*>(tasks_[3].get())->sobserver);

        //ask the state thread to update the state observers
        state_thread->update_state_observers();

//        // create a request so that we print the state
//        // when the ClientTask starts
//        requests_.push_item(PRINT);

//        // we also need to add a response so that the
//        // ClientTask asks  the user to set the goal
//        // enters the goal
//        requests_.push_item(SET_GOAL_SERVER);

//        path_cstr_task_t* pconstruct_thread = static_cast< path_cstr_task_t*>(tasks_[1].get());
//        pconstruct_thread->attach_path_observer(static_cast<path_follow_task_t*>(tasks_[2].get())->pobserver);
//        pconstruct_thread->attach_path_observer(static_cast<request_task_t*>(tasks_[3].get())->pobserver);

    }

    // create a request so that we print the state
    // when the ClientTask starts
    requests_.push_item(PRINT);

    // we also need to add a response so that the
    // ClientTask asks  the user to set the goal
    // enters the goal
    requests_.push_item(SET_GOAL_SERVER);

    // add the tasks... this is where tasks
    // get executed
    thread_pool_.add_tasks(tasks_);

    while(!this->should_stop()){
        tasks_stopped();
    }

    std::lock_guard<std::mutex> lock(msg_mutex);
    std::cout<<"MESSAGE: Task "+this->get_name()<<" exited simulation loop..."<<std::endl;

#ifdef USE_LOG
    kernel::Logger::log_info("Stopping Server Thread");
#endif
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
    kernel::Logger::set_log_file_name("/home/david/CubicEngineering/cubic_engine_develop_branches/integrate_diff_drive_model/cubic_engine/build/examples/exe19/log_file.log");
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

        //writer.write_mesh(mesh, occupamcy_prob, "OccupancyProb");

        // build the mesh graph
        kernel::build_mesh_graph(mesh, map);
    }

    const uint_t N_THREADS = 5;
    const real_t RADIUS = 2.0/100.0;

    // the object that controls when to stop
    // the threads
    StopSimulation stop_sim;

    // the initial system state
    SysState<4> init_state;

    // the vehicle the simulator is simulating
    DiffDriveVehicle vehicle(RADIUS);

    // a wrapper class to provide synchronization
    DiffDriveVehicleWrapper vehicle_wrapper(vehicle);

    ThreadPoolOptions options;
    options.n_threads = N_THREADS;
    options.msg_on_start_up = true;
    options.msg_on_shut_down = true;
    options.msg_when_adding_tasks = true;
    options.start_on_construction = true;

    // executor
    ThreadPool pool(options);

    // the server instance
    ServerThread server(stop_sim, map, init_state, vehicle_wrapper, pool);

    server.run();

    // add the server to the pool
    //pool.add_task(server);

    /*while(true){

    }*/

    //this should block
    pool.close();

    return 0;
}





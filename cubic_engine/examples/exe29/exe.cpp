#include "cubic_engine/base/cubic_engine_types.h"
#include "cubic_engine/search/rapidly_exploring_random_tree.h"
#include "cubic_engine/search/a_star_search.h"
#include "kernel/utilities/csv_file_writer.h"
#include "kernel/dynamics/system_state.h"
#include "kernel/maths/constants.h"
#include "kernel/base/unit_converter.h"
#include "kernel/utilities/common_uitls.h"

#include <cmath>
#include <iostream>
#include <vector>
#include <random>
#include <string>
#include <fstream>

namespace example
{

using cengine::uint_t;
using cengine::real_t;
using cengine::DynMat;
using cengine::DynVec;
using cengine::search::RRT;
using kernel::dynamics::SysState;

const real_t TOL = 1.0e-8;
const real_t DT = 0.5;
const uint_t Nx = 50;
const uint_t Ny = 50;
const real_t Dx = 50.0/Nx;
const real_t xstart = 0.0;
const real_t Dy = 50.0/Ny;
const real_t ystart = 0.0;

struct StateWrapper;

typedef std::vector<StateWrapper> world_t;

struct StateWrapper
{
    SysState<2> state;
    real_t gcost=std::numeric_limits<real_t>::max();
    real_t fcost=std::numeric_limits<real_t>::max();
    bool can_move()const{return true;}
};

struct Metric
{
    typedef real_t cost_t;

    template<typename Node>
    real_t operator()(const Node& s1, const Node& s2 )const{
        return l2Norm(s1.data.state.as_vector()-s2.data.state.as_vector());
    }
};

// create the world
void create_world(world_t& world){

    world.resize(Nx*Ny);

    uint_t counter = 0;
    for(uint_t i =0; i<Nx; ++i){
        for(uint_t j=0; j<Ny; ++j){
          auto& state = world[counter++];
          state.state.set(0, {"X", xstart + i*Dx});
          state.state.set(1, {"Y", ystart + j*Dy});
        }
    }
}

void save_vertices(const RRT<StateWrapper, DynVec<real_t>>& rrt,
                   const std::string& filename){

    // open the file stream
    std::ofstream file;
    file.open(filename, std::ios_base::out);

    for(uint_t v=0; v<rrt.n_vertices(); ++v){
        auto& vertex = rrt.get_vertex(v);
        auto x = vertex.data.state.get("X");
        auto y = vertex.data.state.get("Y");
        file<<vertex.id<<","<<x<<","<<y<<std::endl;

    }
    file.close();
}

void save_connections(const RRT<StateWrapper, DynVec<real_t>>& rrt,
                      const std::string& filename){

    // open the file stream
    std::ofstream file;
    file.open(filename, std::ios_base::out);

    for(uint_t v=0; v<rrt.n_vertices(); ++v){

        auto vneighs = rrt.get_vertex_neighbors(v);
        auto& vertex = rrt.get_vertex(v);
        auto start = vneighs.first;
        auto end = vneighs.second;

        file<<vertex.id<<",";
        while(start != end){
            auto& neigh = rrt.get_vertex(start);

            if (boost::next(start) != end){
                    file<<neigh.id<<",";
            }
            else{
               file<<neigh.id<<"\n";
            }

            ++start;
        }
    }

    file.close();

}

void test_1(){


    typedef RRT<StateWrapper, DynVec<real_t>>::vertex_t Node;

    // the world we work on
    world_t world;

    // create the nodes of the world
    create_world(world);

    std::cout<<"For test_1 world size: "<<world.size()<<std::endl;

    // how to select a state from the world
    auto state_selector = [&world](){

        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> distrib(0, world.size()-1);
        return world[distrib(gen)];
    };


    // compute the dynamics of the model. The tree simply
    // grows in the 45 degrees diagonal
    auto dynamics = [](const Node& s1, const Node& s2){
       StateWrapper s;
       s.state.set(0, {"X", 0.0});
       s.state.set(1, {"Y", 0.0});
       s.state["X"] = s1.data.state["X"] + 0.5;
       s.state["Y"] = s1.data.state["Y"] + 0.5;

       return std::make_tuple(s, DynVec<real_t>(2, 0.0));
    };

    Metric metric;
    Node xinit;

    // build the rrt
    StateWrapper sinit;
    sinit.state.set(0, {"X", 25.0});
    sinit.state.set(1, {"Y", 25.0});

    xinit.data = sinit;

    RRT<StateWrapper, DynVec<real_t>> rrt;
    rrt.set_show_iterations_flag(true);
    rrt.build(world.size(), xinit, state_selector, metric, dynamics);
    save_vertices(rrt, "rrt_test_1.txt");
    save_connections(rrt, "rrt_connections_test_1.txt");
}

void test_2(){

    typedef RRT<StateWrapper, DynVec<real_t>>::vertex_t Node;

    // the world we work on
    world_t world;

    // create the nodes of the world
    create_world(world);

    std::cout<<"For test_1 world size: "<<world.size()<<std::endl;

    // how to select a state from the world
    auto state_selector = [&world](){

        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> distrib(0, world.size()-1);
        return world[distrib(gen)];
    };

    // compute the dynamics of the model.
    auto dynamics = [](const Node& s1, const Node& s2){
       StateWrapper s;
       s.state.set(0, {"X", 0.0});
       s.state.set(1, {"Y", 0.0});
       s.state["X"] = s1.data.state["X"] + (s2.data.state["X"] - s1.data.state["X"]);
       s.state["Y"] = s1.data.state["Y"] + (s2.data.state["Y"] - s1.data.state["Y"]);

       return std::make_tuple(s, DynVec<real_t>(2, 0.0));
    };

    Metric metric;
    Node xinit;

    // build the rrt
    StateWrapper sinit;
    sinit.state.set(0, {"X", 25.0});
    sinit.state.set(1, {"Y", 25.0});

    xinit.data = sinit;

    RRT<StateWrapper, DynVec<real_t>> rrt;
    rrt.set_show_iterations_flag(true);
    rrt.build(1000, xinit, state_selector, metric, dynamics);
    save_vertices(rrt, "rrt_test_2.txt");
    save_connections(rrt, "rrt_connections_test_2.txt");

}

void test_3(){


    typedef RRT<StateWrapper, DynVec<real_t>>::vertex_t Node;

    // the world we work on
    world_t world;

    // create the nodes of the world
    create_world(world);

    std::cout<<"For test_1 world size: "<<world.size()<<std::endl;

    // how to select a state from the world
    auto state_selector = [&world](){

        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> distrib(0, world.size()-1);
        return world[distrib(gen)];
    };

    // compute the dynamics of the model.
    auto dynamics = [](const Node& s1, const Node& s2){
       StateWrapper s;
       s.state.set(0, {"X", 0.0});
       s.state.set(1, {"Y", 0.0});
       s.state["X"] = s1.data.state["X"] + (s2.data.state["X"] - s1.data.state["X"]);
       s.state["Y"] = s1.data.state["Y"] + (s2.data.state["Y"] - s1.data.state["Y"]);

       return std::make_tuple(s, DynVec<real_t>(2, 0.0));
    };

    Metric metric;
    RRT<StateWrapper, DynVec<real_t>> rrt;

    Node xinit;

    // build the rrt
    StateWrapper sinit;
    sinit.state.set(0, {"X", 25.0});
    sinit.state.set(1, {"Y", 25.0});

    xinit.data = sinit;

    Node xgoal;

    StateWrapper sgoal;
    sgoal.state.set(0, {"X", 50.0});
    sgoal.state.set(1, {"Y", 0.0});

    xgoal.data = sgoal;


    rrt.set_show_iterations_flag(true);
    auto [found, root_id, goal_id] = rrt.build(3000, xinit, xgoal, state_selector, metric, dynamics, 2.0);
    save_vertices(rrt, "rrt_test_3.txt");
    save_connections(rrt, "rrt_connections_test_3.txt");

    if(found){

        std::cout<<"Root id: "<<root_id<<std::endl;
        std::cout<<"Goal id: "<<goal_id<<std::endl;

        auto& root = rrt.get_vertex(root_id);
        auto& goal = rrt.get_vertex(goal_id);

        std::cout<<"Goal coords: "<<goal.data.state.get("X")<<","<<goal.data.state.get("Y")<<std::endl;
        auto path_connections = cengine::astar_search(rrt, goal, root, metric);

        auto b = path_connections.begin();
        auto e = path_connections.end();

        std::cout<<"Size of path connections: "<<path_connections.size()<<std::endl;
        auto path = cengine::reconstruct_a_star_path(path_connections, root_id);

        // open the file stream
        std::ofstream vfile;
        vfile.open("rrt_test_3_path_v.txt", std::ios_base::out);

        for(uint_t v=0; v<path.size(); ++v){
            auto& vertex = rrt.get_vertex(path[v]);
            auto x = vertex.data.state.get("X");
            auto y = vertex.data.state.get("Y");
            vfile<<vertex.id<<","<<x<<","<<y<<std::endl;

        }
        vfile.close();
    }
    else{
        std::cout<<"Goal was not found"<<std::endl;
    }
}

}

int main() {
   
    using namespace example;

    try{

        //test_1();
        //test_2();
        test_3();

    }
    catch(std::runtime_error& e){
        std::cerr<<"Runtime error: "
                 <<e.what()<<std::endl;
    }
    catch(std::logic_error& e){
        std::cerr<<"Logic error: "
                 <<e.what()<<std::endl;
    }
    catch(...){
        std::cerr<<"Unknown exception was raised whilst running simulation."<<std::endl;
    }
   
    return 0;
}


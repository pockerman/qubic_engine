#include "cubic_engine/base/cubic_engine_types.h"
#include "cubic_engine/search/rapidly_exploring_random_tree.h"
#include "kernel/utilities/csv_file_writer.h"
#include "kernel/dynamics/system_state.h"
#include "kernel/maths/constants.h"
#include "kernel/base/unit_converter.h"
#include "kernel/utilities/common_uitls.h"
#include "kernel/geometry/geom_point.h"

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

typedef std::vector<SysState<2>> world_t;

// create the world
void create_world(world_t& world){

    world.resize(Nx*Ny);

    uint_t counter = 0;
    for(uint_t i =0; i<Nx; ++i){
        for(uint_t j=0; j<Ny; ++j){
          auto& state = world[counter++];
          state.set(0, {"X", xstart + i*Dx});
          state.set(1, {"Y", ystart + j*Dy});
        }
    }
}

void save(const RRT<SysState<2>, DynVec<real_t>>& rrt,
          const std::string& filename){

    // open the file stream
    std::ofstream file;
    file.open(filename, std::ios_base::out);

    file<<rrt.n_vertices()<<std::endl;

    for(uint_t v=0; v<rrt.n_vertices(); ++v){
        auto vertex = rrt.get_vertex(v);
        auto x = vertex.data.get("X");
        auto y = vertex.data.get("Y");
        file<<vertex.id<<","<<x<<","<<y<<std::endl;
    }

    file<<rrt.n_edges()<<std::endl;

    /*for(uint_t v=0; v<rrt.n_vertices(); ++v){
        auto vertex = rrt.get_vertex(v);
        auto adjacency_list = rrt.get_vertex_neighbors(vertex.id);
        auto begin = adjacency_list.first;
        auto end = adjacency_list.second;
        file<<vertex.id<<",";

        while(begin != end){
            auto nv = *begin;
          file<<nv.data.id<<",";
        }


    }*/

    //auto edges = rrt.edges();
    //auto edges_b = edges.first;
    //auto edges_e = edges.second;

    /*while(edges_b != edges_e){

        auto edge = *edges_b;
        file<<edge.get_id()<<","<<edge.start().id<<","<<edge.end().id<<std::endl;
        edges_b++;
    }*/

    file.close();

}

}

int main() {
   
    using namespace example;

    kernel::CSVWriter writer("state", kernel::CSVWriter::default_delimiter(), true);
    std::vector<std::string> names{"Time", "X", "Y"};
    writer.write_column_names(names);

    try{

        world_t world;

        // create the nodes of the world
        create_world(world);

        std::cout<<"World size: "<<world.size()<<std::endl;

        // how to select a state from the world
        auto state_selector = [&world](){

            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_int_distribution<> distrib(0, world.size()-1);
            return world[distrib(gen)];
        };

        // select the input that minimizes the
        // distance between the two states
        auto input_selector = [](const SysState<2>& s1, const SysState<2>& s2){
            return DynVec<real_t>(2, 0.0);
        };

        // compute the dynamics of the model
        auto dynamics = [](const SysState<2>& s1, const DynVec<real_t>& u){
           SysState<2> s;
           s.set(0, {"X", 0.0});
           s.set(1, {"Y", 0.0});
           s["X"] = s1["X"] + 0.5;
           s["Y"] = s1["Y"] + 0.5;
           return s;
        };

        auto metric = [](const SysState<2>& s1, const SysState<2>& s2 ){
            return l2Norm(s1.as_vector()-s2.as_vector());
        };

        // build the rrt
        SysState<2> xinit;
        xinit.set(0, {"X", 25.0});
        xinit.set(1, {"Y", 25.0});

        RRT<SysState<2>, DynVec<real_t>> rrt;
        rrt.set_show_iterations_flag(true);
        rrt.build(world.size(), xinit, state_selector, input_selector, metric, dynamics);
        save(rrt, "rrt.txt");

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


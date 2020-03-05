#include "kernel/base/types.h"
#include "kernel/discretization/mesh.h"
#include "kernel/discretization/quad_mesh_generation.h"
#include "kernel/data_structs/boost_serial_graph.h"
#include "kernel/data_structs/serial_graph_builder.h"

#include "kernel/maths/functions/numeric_scalar_function.h"
#include "kernel/numerics/boundary_function_base.h"

#include <iostream>

namespace example
{
using kernel::real_t;
using kernel::uint_t;
using kernel::Null;
using kernel::numerics::Mesh;
using kernel::GeomPoint;
using kernel::BoostSerialGraph;

class UCompBC: public kernel::numerics::BoundaryFunctionBase<2>
{
public:

    typedef kernel::numerics::BoundaryFunctionBase<2>::input_t input_t;
    typedef kernel::numerics::BoundaryFunctionBase<2>::output_t output_t;

    /// constructor
    UCompBC();

    /// \brief Returns the value of the function on the Dirichlet boundary
    virtual output_t value(const GeomPoint<2>&  /*input*/)const override final;
};

UCompBC::UCompBC()
    :
    kernel::numerics::BoundaryFunctionBase<2>()
{
    this->set_bc_type(0, kernel::numerics::BCType::ZERO_DIRICHLET);
    this->set_bc_type(1, kernel::numerics::BCType::ZERO_DIRICHLET);
    this->set_bc_type(2, kernel::numerics::BCType::DIRICHLET);
    this->set_bc_type(3, kernel::numerics::BCType::ZERO_DIRICHLET);
}

UCompBC::output_t
UCompBC::value(const GeomPoint<2>&  p)const{

    if(p[1] == 1.0){
        return 2.0;
    }

    return 0.0;
}

class VCompBC: public kernel::numerics::BoundaryFunctionBase<2>
{
public:

    typedef kernel::numerics::BoundaryFunctionBase<2>::input_t input_t;
    typedef kernel::numerics::BoundaryFunctionBase<2>::output_t output_t;

    /// constructor
    VCompBC();

    /// \brief Returns the value of the function on the Dirichlet boundary
    virtual output_t value(const GeomPoint<2>&  /*input*/)const override final;
};

VCompBC::VCompBC()
    :
    kernel::numerics::BoundaryFunctionBase<2>()
{
    this->set_bc_type(0, kernel::numerics::BCType::ZERO_DIRICHLET);
    this->set_bc_type(1, kernel::numerics::BCType::ZERO_DIRICHLET);
    this->set_bc_type(2, kernel::numerics::BCType::ZERO_DIRICHLET);
    this->set_bc_type(3, kernel::numerics::BCType::ZERO_DIRICHLET);
}

VCompBC::output_t
VCompBC::value(const GeomPoint<2>&  p)const{return 0.0;}


}

int main(){

    using namespace example;

    try{

        Mesh<2> mesh;

        {
            uint_t nx = 2;
            uint_t ny = 2;

            GeomPoint<2> start(0.0);
            GeomPoint<2> end(1.0);

            std::cout<<"Starting point: "<<start<<std::endl;
            std::cout<<"Ending point: "<<end<<std::endl;

            // generate the mesh
            kernel::numerics::build_quad_mesh(mesh, nx, ny, start, end);

            std::cout<<"MESH STATISTICS "<<std::endl;
            std::cout<<"Number of elements: "<<mesh.n_elements()<<std::endl;
            std::cout<<"Number of nodes: "<<mesh.n_nodes()<<std::endl;
            std::cout<<"Number of faces: "<<mesh.n_faces()<<std::endl;
            std::cout<<"Number of edges: "<<mesh.n_edges()<<std::endl;
            std::cout<<"Number of boundaries: "<<mesh.n_boundaries()<<std::endl;
            std::cout<<"\n";
        }

        // create the graph from the mesh
        BoostSerialGraph<Null, Null> graph;

        // build the mesh graph
        build_mesh_graph(mesh, graph);

        std::cout<<"Number of vertices: "<<graph.n_vertices()<<std::endl;
        std::cout<<"Number of edges: "<<graph.n_edges()<<std::endl;
    }
    catch(std::logic_error& error){

        std::cerr<<error.what()<<std::endl;
    }
    catch(...){
        std::cerr<<"Unknown exception occured"<<std::endl;
    }

    return 0;
}






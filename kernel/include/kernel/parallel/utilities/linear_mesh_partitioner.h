#ifndef LINEAR_MESH_PARTITIONER_H
#define LINEAR_MESH_PARTITIONER_H

#include "kernel/base/types.h"
#include "kernel/base/config.h"
#include "kernel/discretization/mesh.h"
#include "kernel/discretization/element.h"
#include "kernel/discretization/element_mesh_iterator.h"
#include "kernel/discretization/mesh_predicates.h"
#include "kernel/discretization/mesh_tools.h"

#ifdef USE_LOG
#include "kernel/utilities/logger.h"
#include <chrono>
#include <sstream>
#endif

#include <iterator> // for std::distance

namespace kernel{
namespace numerics {

template<int dim> class Mesh;


/// \brief  Assigns the processor id for every element in the
/// given Mesh object
template<int dim>
void linear_mesh_partition(Mesh<dim>& mesh, uint_t n_parts){

    uint_t total_work = n_active_elements(mesh);

    if(n_parts == 0){
        throw std::invalid_argument("Cannot partition range into zero parts");
    }

    if(total_work == 0){
        throw std::invalid_argument("Cannot partition a range with equal start and end points");
    }

#ifdef USE_LOG
    std::chrono::time_point<std::chrono::system_clock> start_timing = std::chrono::system_clock::now();
#endif


    //how many elements each partition will get
    uint_t part_load = total_work/n_parts;
    uint_t next_available_pe_id = 0;
    uint_t loaded = 0;

    ElementMeshIterator<Active, Mesh<dim>> filter(mesh);
    auto element_begin = filter.begin();
    auto element_end = filter.end();

    for(; element_begin != element_end; ++element_begin){

        auto* element = *element_begin;
        element->set_pid(next_available_pe_id);
        ++loaded;

        // last processor takes all of what is left
        if(loaded == part_load && next_available_pe_id <n_parts-1){
            next_available_pe_id++;
            loaded = 0;
        }
    }

#ifdef USE_LOG
    std::chrono::time_point<std::chrono::system_clock> end_timing = std::chrono::system_clock::now();
    std::chrono::duration<real_t> dur = end_timing-start_timing;
    std::ostringstream message;
    message<<"linear_mesh_partition run time: "<<dur.count();
    Logger::log_info(message.str());
#endif
}

}
}

#endif // LINEAR_MESH_PARTITIONER_H

#ifndef A_STAR_SEARCH_H
#define A_STAR_SEARCH_H

#include "cubic_engine/base/cubic_engine_types.h"
#include "kernel/data_structs/searchable_priority_queue.h"
#include "kernel/utilities/map_utilities.h"

#include <utility>
#include <set>
#include <queue>
#include <map>

namespace cengine
{
    namespace astar_impl
    {

        struct fcost_astar_node_compare
        {
           template<typename NodeTp>
           bool operator()(const NodeTp& n1,const NodeTp& n2)const;
        };

        template<typename NodeTp>
        bool
        fcost_astar_node_compare::operator()(const NodeTp& n1,const NodeTp& n2)const{

           if(n1.data.fcost > n2.data.fcost){
               return true;
           }

           return false;
        }

        struct id_astar_node_compare
        {
           template<typename NodeTp>
           bool operator()(const NodeTp& n1,const NodeTp& n2)const;
        };

        template<typename NodeTp>
        bool
        id_astar_node_compare::operator()(const NodeTp& n1,const NodeTp& n2)const{

           if(n1.id > n2.id){
               return true;
           }

           return false;
        }
    }

/// \brief Simple implementation of A* algorithm
/// at the moment the algorithm is only usable with a
/// boost_unidirected_serial_graph graph
template<typename GraphTp, typename H>
std::multimap<uint_t, uint_t>
astar_search(GraphTp& g, typename GraphTp::vertex_type& start, typename GraphTp::vertex_type& end, const H& h){

   std::multimap<uint_t, uint_t> came_from;

   if(start == end){
     //we don't have to search for anything
    kernel::add_or_update_map(came_from,start.id,start.id);
     return came_from;
   }

   typedef typename H::cost_t cost_t;
   typedef typename GraphTp::vertex_type vertex_t;
   typedef typename GraphTp::adjacency_iterator adjacency_iterator;
   typedef typename GraphTp::vertex_type node_t;

   std::set<node_t,astar_impl::id_astar_node_compare> explored;
   kernel::searchable_priority_queue<node_t, std::vector<node_t>, astar_impl::fcost_astar_node_compare> open;

   //the cost of the path so far leading to this
   //node is obviously zero at the start node
   start.data.gcost = 0.0;

   //calculate the fCost from start node to the goal
   //at the moment this can be done only heuristically
   start.data.fcost = h(start.data.position, end.data.position);
   open.push(start);

   while(!open.empty()){

      //the vertex currently examined
      const node_t cv = open.top();
      open.pop();

      //check if this is the goal
      if(cv == end){
         break;
      }

      //current node is not the goal so proceed
      //add it to the explored (or else called closed) set
      explored.insert(cv);

      //get the adjacent neighbors
      std::pair<adjacency_iterator,adjacency_iterator> neighbors = g.get_vertex_neighbors(cv);
      auto itr = neighbors.first;

      ///loop over the neighbors
      for(; itr != neighbors.second; itr++){

         node_t& nv = g.get_vertex(itr);
         uint_t nid = nv.id;

         //search explored set by id
         auto itr = std::find_if(explored.begin(), explored.end(),
                                 [=](const node_t& n){return (n.id == nid);});

         //the node has been explored
         if(itr != explored.end()){
            continue;
         }

         //this actually the cost of the path from the current node
         //to reach its neighbor
         cost_t tg_cost = cv.data.gcost + h(cv.data.position, nv.data.position);

         if (tg_cost >= nv.data.gcost) {
            continue; //this is not a better path
         }

         // This path is the best until now. Record it!
         kernel::add_or_update_map(came_from,nv.id,cv.id);

         //came_from.put(nv.id,cv.id);
         nv.data.gcost = tg_cost;

         //acutally calculate f(nn) = g(nn)+h(nn)
         nv.data.fcost = nv.data.gcost + h(nv.data.position, end.data.position);

         //update here the open as we copy
         if(!open.contains(nv)){
                 open.push(nv);
         }
      }
   }

   return came_from;
}

template<typename IdTp>
std::vector<IdTp>
reconstruct_a_star_path(const std::multimap<IdTp,IdTp>& map, const IdTp& start){

    if(map.empty()){
        return std::vector<IdTp>();
    }

    std::vector<IdTp> path;
    path.push_back(start);

    auto next_itr = map.find(start);

    if(next_itr == map.end()){
        //such a key does not exist
        throw std::logic_error("Key: "+std::to_string(start)+" does not exist");
    }

    IdTp next = next_itr->second;
    path.push_back(next);

    while(next_itr!=map.end()){

        next_itr = map.find(next);
        if(next_itr != map.end()){
            next = next_itr->second;
            path.push_back(next);
        }
    }

    //let's reverse the path
    std::vector<IdTp> the_path;
    the_path.reserve(path.size());
    auto itrb = path.rbegin();
    auto itre = path.rend();

    while(itrb != itre){
        the_path.push_back(*itrb++);
    }

    return the_path;
}


}

#endif // A_STAR_SEARCH_H

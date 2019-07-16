#ifndef A_STAR_SEARCH_H
#define A_STAR_SEARCH_H

#include "cubic_engine/base/cubic_engine_types.h"

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
template<typename GraphTp,typename H>
std::multimap<size_type,size_type>
astar_search(GraphTp& g, typename GraphTp::vertex_type& start, typename GraphTp::vertex_type& end, const H& h){

   std::multimap<uint_t, uint_t> came_from;

   if(start == end){
     //we don't have to search for anything
     add_or_update_map(came_from,start.id,start.id);
     return came_from;
   }

   typedef typename H::cost_t cost_t;
   typedef typename GraphTp::vertex_type vertex_t;
   typedef typename GraphTp::adjacency_iterator adjacency_iterator;
   typedef typename GraphTp::vertex_type node_t;

   std::set<node_t,astar_impl::id_astar_node_compare> explored;
   searchable_priority_queue<node_t, std::vector<node_t>, astar_impl::fcost_astar_node_compare> open;

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
         size_type nid = nv.id;

         //search explored set by id
         auto itr = std::find_if(explored.begin(), explored.end(),
                                 [=](const node_t& n){return (n.id == nid);});

         //the node has been explored
         if(itr != explored.end()){
                 continue;
         }

         //this actually the cost of the path from the current node
         //to reach its neighbor
         cost_t tgCost = cv.data.gcost + h(cv.data.position, nv.data.position);

         if (tgCost >= nv.data.gcost) {
            continue; //this is not a better path
         }

         // This path is the best until now. Record it!
         add_or_update_map(came_from,nv.id,cv.id);

         //came_from.put(nv.id,cv.id);
         nv.data.gcost = tgCost;

         //acutally calculate f(nn) = g(nn)+h(nn)
         nv.data.fcost = nv.data.gcost + h(nv.data.position,end.data.position);

         //update here the open as we copy
         if(!open.contains(nv)){
                 open.push(nv);
         }
      }
   }

   return came_from;
}


}

#endif // A_STAR_SEARCH_H

#ifndef RAPIDLY_EXPLORING_RANDOM_TREE_H
#define RAPIDLY_EXPLORING_RANDOM_TREE_H

#include "cubic_engine/base/cubic_engine_types.h"
#include "kernel/data_structs/boost_serial_graph.h"

#include"boost/noncopyable.hpp"
#include <chrono>
#include <iostream>


namespace cengine {
namespace search {

///
/// \brief The RRT class models a Rapidly-Exploring Random Tree
/// see: http://msl.cs.uiuc.edu/~lavalle/papers/Lav98c.pdf
/// The NodeData most frequently will represent the
/// state type modeled. It corresponds to the types
/// of \f$x_{rand}\f$ and \f$x_{new}\f$ from the paper cited above.
/// The EdgeData type corresponds to the type of \f$u\f$ in the paper.
/// It is the input that should subsequently be applied to reach from
/// one state to another and this is what the applications most often will use
///
template<typename NodeData, typename EdgeData>
class RRT: private boost::noncopyable
{
public:

    ///
    /// \brief node_data_t The type of node data
    ///
    typedef NodeData node_data_t;

    ///
    /// \brief edge_data_t The type of the edge data
    ///
    typedef EdgeData edge_data_t;

    ///
    /// \brief node_t the vertex type
    ///
    typedef typename kernel::BoostSerialGraph<node_data_t,
                                              edge_data_t>::vertex_type node_t;

    ///
    /// \brief edge_t The edge type
    ///
    typedef typename kernel::BoostSerialGraph<node_data_t,
                                              edge_data_t>::edge_type edge_t;

    ///
    /// \brief edge_iterator Edge iterator
    ///
    typedef typename kernel::BoostSerialGraph<node_data_t,
                                              edge_data_t>::edge_iterator edge_iterator;

    ///
    /// \brief adjacency_iterator Adjacency iterator
    ///
    typedef typename kernel::BoostSerialGraph<node_data_t,
                                              edge_data_t>::adjacency_iterator adjacency_iterator;

    ///
    /// \brief RRT Default constructor. Creates an empty tree
    ///
    RRT();

    ///
    /// \brief get_vertex Returns the v-th vertex
    ///
    node_t& get_vertex(uint_t v){ return tree_.get_vertex(v);}

    ///
    /// \brief get_vertex Returns the v-th vertex
    ///
    const node_t& get_vertex(uint_t v)const{return tree_.get_vertex(v);}

    ///
    /// \brief Returns the neighboring vertices for the given vertex id
    ///
    std::pair<adjacency_iterator, adjacency_iterator>
    get_vertex_neighbors(uint_t id)const{
        return tree_.get_vertex_neighbors(id);
    }

    ///
    /// \brief add_vertex Add a new vertex to the tree
    /// \param node The new vertex to add
    ///
    node_t& add_vertex(const node_t& node){ return tree_.add_vertex(node.data);}

    ///
    /// \brief Add a new vertex in the tree that has the given data
    ///
    node_t& add_vertex(const node_data_t& data);

    ///
    /// \brief get_edge Returns  the edge between the vertices v1 and v2
    /// \param v1 Vertex 1
    /// \param v2 Vertex 2
    ///
    edge_t& get_edge(uint_t v1, uint_t v2){
        return tree_.get_edge(v1, v2);
    }

    ///
    /// \brief get_edge Returns  the edge between the vertices v1 and v2
    /// \param v1 Vertex 1
    /// \param v2 Vertex 2
    ///
    const edge_t& get_edge(uint_t v1, uint_t v2)const{
        return tree_.get_edge(v1, v2);
    }

    ///
    /// \brief edges Access the edges of the tree
    ///
    std::pair<edge_iterator, edge_iterator> edges()const{
        return tree_.edges();
    }

    ///
    /// \brief add_edge Add a new edge between the vertices v1 and v2
    /// \param v1 Vertex 1
    /// \param v2 Vertex 2
    ///
    edge_t& add_edge(uint_t v1, uint_t v2){
        return tree_.add_edge(v1, v2);
    }

    ///
    /// \brief find_nearest_neighbor find the nearest neighbor of other node in this tree
    /// \param other The node for which to find the nearest neighbor
    /// \return
    ///
    template<typename MetricTp>
    const node_t& find_nearest_neighbor(const node_t& other,
                                        const MetricTp& metric)const;

    ///
    /// \brief clear Clear the underlying tree
    ///
    void clear(){tree_.clear();}

    ///
    /// \brief n_vertices. Returns the number of vertices of the tree
    /// \return
    ///
    uint_t n_vertices()const{return tree_.n_vertices();}

    ///
    /// \brief n_edges. Returns the number of edges of the tree
    /// \return
    ///
    uint_t n_edges()const{return tree_.n_edges();}

    ///
    /// \brief set_show_iterations_flag Set the show_iterations_ flag
    ///
    void set_show_iterations_flag(bool val){show_iterations_ = val;}

    ///
    /// \brief Build the tree
    ///
    template<typename StateSelector, typename InputSelector,
             typename MetricTp, typename DynamicsTp>
    void build(uint_t nitrs, const node_t& xinit,
               const  StateSelector& state_selector,
               const InputSelector& input_selector,
               const MetricTp& metric,
               DynamicsTp& dynamics);

private:

    ///
    /// \brief tree_ The underlying tree data structure
    ///
    kernel::BoostSerialGraph<node_data_t, edge_data_t> tree_;

    ///
    /// \brief show_iterations_ Flag indicating if information
    /// on the iterations should be displayed
    ///
    bool show_iterations_;

};

template<typename NodeData, typename EdgeData>
RRT<NodeData, EdgeData>::RRT()
    :
      tree_(),
      show_iterations_(false)
{}

template<typename NodeData, typename EdgeData>
typename RRT<NodeData, EdgeData>::node_t&
RRT<NodeData, EdgeData>::add_vertex(const node_data_t& data){
    return tree_.add_vertex(data);
}

template<typename NodeTp, typename EdgeTp>
template<typename StateSelector, typename InputSelector,
         typename MetricTp, typename DynamicsTp>
void
RRT<NodeTp, EdgeTp>::build(uint_t nitrs, const node_t& xinit,
                           const  StateSelector& state_selector,
                           const InputSelector& input_selector,
                           const MetricTp& metric,
                           DynamicsTp& dynamics){

    std::chrono::time_point<std::chrono::system_clock> start, end;
    start = std::chrono::system_clock::now();

    // initialize the tree
    auto root = tree_.add_vertex(xinit.data);

    // loop over the states and create
    // the tree
    for(uint_t itr=0; itr<nitrs; ++itr){

        if(show_iterations_){
            std::cout<<"At iteration: "<<itr<<std::endl;
        }

        // select a new random state
        auto xrand = state_selector();


        std::cout<<xrand.get("X")<<","<<xrand.get("Y")<<std::endl;

        // find the nearest neighbor between this tree
        // and the randomly selected state
        auto xnear = find_nearest_neighbor(xrand, metric);

        // select the input
        auto u = input_selector(xrand, xnear.data);

        // determine the new state
        auto xnew = dynamics(xnear.data, u);

        // add a new vertex
        auto new_v = add_vertex(xnew);

        // add a new edge
        auto new_e = add_edge(xnear.id, new_v.id);
        new_e.set_data(u);
    }

    end = std::chrono::system_clock::now();

    if(show_iterations_){
        std::chrono::duration<real_t> dur = end - start;
        std::cout<<"Total build time: "<<dur.count()<<std::endl;
    }
}

template<typename NodeData, typename EdgeData>
template<typename MetricTp>
const typename RRT<NodeData, EdgeData>::node_t&
RRT<NodeData, EdgeData>::find_nearest_neighbor(const node_t& other,
                                               const MetricTp& metric)const{

    auto dist = metric(tree_.get_vertex(0).data, other.data);
    auto result = 0;

    for(uint_t v=1; v< tree_.n_vertices(); ++v){
        auto vertex_data = tree_.get_vertex(v).data;

        auto new_dist = metric(vertex_data, other.data);

        if(new_dist < dist){
            dist = new_dist;
            result = v;
        }
    }

    return tree_.get_vertex(result);
}



}

}

#endif // RAPIDLY_EXPLORING_RANDOM_TREE_H

#ifndef RAPIDLY_EXPLORING_RANDOM_TREE_H
#define RAPIDLY_EXPLORING_RANDOM_TREE_H

#include "cubic_engine/base/cubic_engine_types.h"
#include "kernel/data_structs/boost_serial_graph.h"

#include"boost/noncopyable.hpp"

namespace cengine {
namespace search {

///
/// \brief The RRT class models a Rapidly-Exploring Random Tree
/// see: http://msl.cs.uiuc.edu/~lavalle/papers/Lav98c.pdf
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
    typedef typename kernel::BoostSerialGraph<node_data_t, edge_data_t>::vertex_type node_t;

    ///
    /// \brief edge_t The edge type
    ///
    typedef typename kernel::BoostSerialGraph<node_data_t, edge_data_t>::edge_type edge_t;

    ///
    /// \brief RRT Default constructor. Creates an empty tree
    ///
    RRT();

    ///
    /// \brief add_vertex Add a new vertex to the tree
    /// \param node The new vertex to add
    ///
    void add_vertex(const node_t& node);

    ///
    /// \brief add_edge Add a new edge between the vertices v1 and v2
    /// \param v1 Vertex 1
    /// \param v2 Vertex 2
    ///
    void add_edge(uint_t v1, uint_t v2);

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

    template<typename StateSelector, typename InputSelector, typename DynamicsTp>
    void build(uint_t nitrs, const node_t& xinit,
               const  StateSelector& state_selector,
               const InputSelector& input_selector,
               DynamicsTp& dynamics);

private:

    ///
    /// \brief tree_ The underlying tree data structure
    ///
    kernel::BoostSerialGraph<node_data_t, edge_data_t> tree_;

};

template<typename NodeData, typename EdgeData>
RRT<NodeData, EdgeData>::RRT()
    :
      tree_()
{}

template<typename NodeTp, typename EdgeTp>
template<typename StateSelector, typename InputSelector, typename DynamicsTp>
void
RRT<NodeTp, EdgeTp>::build(uint_t nitrs, const node_t& xinit,
                           const  StateSelector& state_selector,
                           const InputSelector& input_selector,
                           DynamicsTp& dynamics){

    // initialize the tree

    for(uint_t itr=0; itr<nitrs; ++itr){

        // select a new random state
        auto xrand = state_selector();

        // find the nearest neighbor between this tree
        // and the randomly selected state
        auto xnear = find_nearest_neighbor(xrand);

        // select the input
        auto u = input_selector(xrand, xnear);

        // determine the new state
        auto xnew = dynamics(xnear, u);

        // add a new vertex
        add_vertex(xnew);

        // add a new edge
        add_edge(xnear, xnew, u);
    }
}

template<typename NodeData, typename EdgeData>
template<typename MetricTp>
const typename RRT<NodeData, EdgeData>::node_t&
RRT<NodeData, EdgeData>::find_nearest_neighbor(const node_t& other,
                                               const MetricTp& metric)const{

    // loop over the vertices and apply
    // the metric
    auto dist = metric(tree_.get_vertex(0), other);
    auto result = tree_.get_vertex(0);


    return result;


}

}

}

#endif // RAPIDLY_EXPLORING_RANDOM_TREE_H

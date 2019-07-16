#ifndef BOOST_SERIAL_GRAPH_H
#define BOOST_SERIAL_GRAPH_H

#include "parframe/base/types.h"
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>

namespace kernel
{

/// \detailed. Representation of a graph using adjacency lists.
/// The underlying implementation uses Boost Graph library.
/// This wrapper is introduced to simplify the creation
/// and handling of the graph.

template<typename VertexTp,typename EdgeTp>
class BoostSerialGraph
{

public:

        typedef VertexTp vertex_type;
        typedef EdgeTp   edge_type;

private:

        typedef boost::adjacency_list<boost::vecS, boost::vecS,
                                      boost::bidirectionalS, vertex_type,edge_type> graph_type;

public:

        typedef typename boost::graph_traits<graph_type>::vertex_descriptor vertex_t;
        typedef typename boost::graph_traits<graph_type>::edge_descriptor edge_t;

        /// \brief Constructor
        BoostSerialGraph(uint_t nvs);

        /// \brief Add a vertex to the graph and get back a vertex
        /// decriptor
        vertex_t add_vertex(const vertex_type& v);

        /// \brief Add an edge formed by the two given vertices
        edge_type& add_edge(uint_t v1, uint_t v2);

        /// \brief Returns the number of vertices
        uint_t n_vertices()const{return g_.size();}

private:

        /// \brief The actual graph
        graph_type g_;
};

template<typename VertexTp,typename EdgeTp>
BoostSerialGraph<VertexTp,EdgeTp>::BoostSerialGraph(uint_t nv)
:
g_(nv)
{}

template<typename VertexTp,typename EdgeTp>
typename BoostSerialGraph<VertexTp,EdgeTp>::vertex_t
BoostSerialGraph<VertexTp,EdgeTp>::add_vertex(const typename BoostSerialGraph<VertexTp,EdgeTp>::vertex_type& v){

  vertex_t a = boost::add_vertex(g_);

  //access the vertex and set up the data
  // from the given vertex
  g_[a] = v;

  //override the id
  return a;

}

template<typename VertexTp,typename EdgeTp>
typename BoostSerialGraph<VertexTp,EdgeTp>::edge_type&
BoostSerialGraph<VertexTp,EdgeTp>::add_edge(uint_t v1, uint_t v2){

  edge_t et;
  bool condition;
  vertex_t a = boost::vertex(v1, g_);
  vertex_t b = boost::vertex(v2, g_);
  boost::tie(et, condition) = boost::add_edge(a, b, g_);
  edge_type& edge = g_[et];
  return edge;
}


}

#endif // BOOST_SERIAL_GRAPH_H

#ifndef BOOST_SERIAL_GRAPH_H
#define BOOST_SERIAL_GRAPH_H

#include "kernel/base/types.h"
#include "kernel/base/kernel_consts.h"
#include "kernel/geometry/generic_line.h"
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/undirected_graph.hpp>

#include <utility>
#include <stdexcept>

namespace kernel{

///
/// \brief. Representation of a graph using adjacency lists.
/// The underlying implementation uses Boost Graph library.
/// This wrapper is introduced to simplify the creation
/// and handling of the graph.
///
template<typename VertexData, typename EdgeData>
class BoostSerialGraph
{

public:

    ///
    /// \brief vertex_data_t The type of the vertex data
    ///
    typedef VertexData vertex_data_t;

    ///
    /// \brief edge_data_t The type of the edge data
    ///
    typedef EdgeData   edge_data_t;

    ///
    /// \brief Class that represents the Node of a graph
    ///
    struct node_t
    {
        vertex_data_t data;
        uint_t  id;

        node_t();
        node_t(const vertex_data_t& data);
        node_t(vertex_data_t&& data);
        node_t(const node_t& o);

        node_t& operator=(const node_t& o);
        bool operator==(const node_t& o)const{return this->id==o.id;}
        bool operator!=(const node_t& o)const{return !(*this==o);}
    };

    ///
    /// \brief vertex_type The vertex type
    ///
    typedef node_t vertex_type;

    ///
    /// \brief edge_type The edge type
    ///
    typedef GenericLine<vertex_type, EdgeData> edge_type;

private:

    typedef boost::undirected_graph<vertex_type, edge_type> graph_type;
    typedef typename boost::graph_traits<graph_type>::vertex_descriptor vertex_t;
    typedef typename boost::graph_traits<graph_type>::edge_descriptor edge_t;

public:

    ///
    /// \brief edge_iterator Edge iterator
    ///
    typedef typename graph_type::edge_iterator edge_iterator;

    ///
    /// \brief adjacency_iterator Adjacency iterator
    ///
    typedef typename graph_type::adjacency_iterator adjacency_iterator;

    ///
    /// \brief Constructor
    ///
    explicit BoostSerialGraph(uint_t nvs=0);

    ///
    /// \brief Add a vertex to the graph by providing the data
    ///
    vertex_type& add_vertex(const VertexData& data);

    ///
    /// \brief Add an edge formed by the two given vertices
    ///
    edge_type& add_edge(uint_t v1, uint_t v2);

    ///
    /// \brief Access the i-th vertex of the graph
    ///
    const vertex_type& get_vertex(uint_t i)const;

    ///
    /// \brief Access the i-th vertex of the graph
    ///
    vertex_type& get_vertex(uint_t i);

    ///
    /// \brief Access the vertex given the vertex descriptor
    /// This is needed when accessing the vertices using the adjacency_iterator
    ///
    vertex_type& get_vertex(adjacency_iterator itr);

    ///
    /// \brief Access the i-th edge of the graph with endpoints
    /// the given vertices
    ///
    const edge_type& get_edge(uint_t v1, uint_t v2)const;

    ///
    /// \brief Access the i-th edge of the graph with endpoints
    /// the given vertices
    ///
    edge_type& get_edge(uint_t v1, uint_t v2);

    ///
    /// \brief edges Access the edges of the tree
    ///
    std::pair<edge_iterator,edge_iterator> edges()const;

    ///
    /// \brief Returns the neighboring vertices for the given vertex id
    ///
    std::pair<adjacency_iterator, adjacency_iterator> get_vertex_neighbors(uint_t id)const;

    ///
    /// \brief Returns the neighboring vertices for the given vertex id
    ///
    std::pair<adjacency_iterator, adjacency_iterator> get_vertex_neighbors(const vertex_type& v)const;

    ///
    /// \brief Returns the number of vertices
    ///
    uint_t n_vertices()const{return g_.num_vertices();}

    ///
    /// \brief Returns the maximum vertex index
    ///
    uint_t max_vertex_index()const noexcept{return g_.max_vertex_index();}

    ///
    /// \brief Returns the number of edges
    ///
    uint_t n_edges()const{return g_.num_edges();}

    ///
    /// \brief Returns the maximum edge index
    ///
    uint_t max_edge_index() const noexcept{return g_.max_edge_index();}

    ///
    /// \brief Clear the graph
    ///
    void clear(){g_.clear();}

private:

    ///
    /// \brief The actual graph
    ///
    graph_type g_;
};

template<typename VertexData,typename EdgeData>
BoostSerialGraph<VertexData,EdgeData>::BoostSerialGraph(uint_t nv)
:
g_(nv)
{}

template<typename VertexData, typename EdgeData>
typename BoostSerialGraph<VertexData,EdgeData>::vertex_type&
BoostSerialGraph<VertexData,EdgeData>::add_vertex(const VertexData& data){

    typedef typename BoostSerialGraph<VertexData,EdgeData>::vertex_type vertex_type;
    typedef typename BoostSerialGraph<VertexData,EdgeData>::vertex_t    vertex_t;
    uint_t idx = n_vertices();

    //add a new vertex
    vertex_t a = boost::add_vertex(g_);
    vertex_type& v = g_[a];
    v.data = data;
    v.id = idx;
    return v;
}

template<typename VertexData,typename EdgeData>
typename BoostSerialGraph<VertexData,EdgeData>::edge_type&
BoostSerialGraph<VertexData,EdgeData>::add_edge(uint_t v1, uint_t v2){

    //TODO what happens when v1 and v2 are not valid vertex indices?
    //at the moment we throw an assertion
    if(v1>=n_vertices() || v2 >=n_vertices())
        throw std::logic_error("Invalid vertex index v1/v2: "+
                                std::to_string(v1)+
                                "/"+
                                std::to_string(v2)+
                                " not in [0,"+
                                std::to_string(n_vertices())+
                                ")");
    typedef typename BoostSerialGraph<VertexData,EdgeData>::vertex_t vertex_t;
    typedef typename BoostSerialGraph<VertexData,EdgeData>::edge_t edge_t;
    typedef typename BoostSerialGraph<VertexData,EdgeData>::edge_type edge_type;
    edge_t et;
    bool condition;

    //get the vertices that correspond to the indices
    vertex_t a = boost::vertex(v1,g_);
    vertex_t b = boost::vertex(v2,g_);
    uint_t idx = n_edges();

    ///create an edge
    boost::tie(et,condition) = boost::add_edge(a,b,g_);
    edge_type& edge = g_[et];
    edge.set_id(idx);
    return edge;
}


template<typename VertexData,typename EdgeData>
const typename BoostSerialGraph<VertexData,EdgeData>::vertex_type&
BoostSerialGraph<VertexData,EdgeData>::get_vertex(uint_t i)const{

    if(i>=n_vertices()){
        throw std::logic_error("Invalid vertex index. Index "+
                                std::to_string(i)+
                                " not in [0,"+
                                std::to_string(n_vertices())+
                                ")");
    }

    typedef typename BoostSerialGraph<VertexData,EdgeData>::vertex_t vertex_t;
    vertex_t a = boost::vertex(i,g_);
    return g_[a];
}

template<typename VertexData,typename EdgeData>
typename BoostSerialGraph<VertexData,EdgeData>::vertex_type&
BoostSerialGraph<VertexData,EdgeData>::get_vertex(adjacency_iterator itr){

    return g_[*itr];
}

template<typename VertexData,typename EdgeData>
typename BoostSerialGraph<VertexData,EdgeData>::vertex_type&
BoostSerialGraph<VertexData,EdgeData>::get_vertex(uint_t i){

    return const_cast<BoostSerialGraph<VertexData,EdgeData>::vertex_type&>(

            static_cast<const BoostSerialGraph<VertexData,EdgeData>&>(*this).get_vertex(i)
    );
}


template<typename VertexData,typename EdgeData>
const typename BoostSerialGraph<VertexData,EdgeData>::edge_type&
BoostSerialGraph<VertexData,EdgeData>::get_edge(uint_t v1, uint_t v2)const{

        //TODO what happens when v1 and v2 are not valid vertex indices?
        //at the moment we throw an assertion
        if(v1>=n_vertices() || v2 >=n_vertices())
            throw std::logic_error("Invalid vertex index v1/v2. Indeces "+
                                    std::to_string(v1)+
                                    "/"+
                                    std::to_string(v2)+
                                    " not in [0,"+
                                    std::to_string(n_vertices())+
                                    ")");

        typedef typename BoostSerialGraph<VertexData,EdgeData>::vertex_t vertex_t;
        typedef typename BoostSerialGraph<VertexData,EdgeData>::edge_t edge_t;
        vertex_t a = boost::vertex(v1,g_);
        vertex_t b = boost::vertex(v2,g_);

        std::pair<edge_t,bool> rslt = boost::edge(a,b,g_);

        return g_[rslt.first];
}


template<typename VertexData,typename EdgeData>
typename BoostSerialGraph<VertexData,EdgeData>::edge_type&
BoostSerialGraph<VertexData,EdgeData>::get_edge(uint_t v1, uint_t v2){

    return const_cast<BoostSerialGraph<VertexData,EdgeData>::edge_type&>(
                static_cast<const BoostSerialGraph<VertexData,EdgeData>&>(*this).get_edge(v1,v2)
        );

}

template<typename VertexData,typename EdgeData>
std::pair<typename BoostSerialGraph<VertexData,EdgeData>::adjacency_iterator,
          typename BoostSerialGraph<VertexData,EdgeData>::adjacency_iterator>
BoostSerialGraph<VertexData,EdgeData>::get_vertex_neighbors(uint_t i)const{

    if(i>=n_vertices()){
            throw std::logic_error("Invalid vertex index. Index "+
                                    std::to_string(i)+
                                    " not in [0,"+
                                    std::to_string(n_vertices())+
                                    ")");
    }

    typedef typename BoostSerialGraph<VertexData,EdgeData>::vertex_t vertex_t;
    vertex_t a = boost::vertex(i,g_);
    return boost::adjacent_vertices(a, g_);
}

template<typename VertexData,typename EdgeData>
std::pair<typename BoostSerialGraph<VertexData,EdgeData>::adjacency_iterator,
          typename BoostSerialGraph<VertexData,EdgeData>::adjacency_iterator>
BoostSerialGraph<VertexData,EdgeData>::get_vertex_neighbors(const typename BoostSerialGraph<VertexData,EdgeData>::vertex_type& v)const{
    return get_vertex_neighbors(v.id);
}

template<typename VertexData,typename EdgeData>
std::pair<typename BoostSerialGraph<VertexData,EdgeData>::edge_iterator,
          typename BoostSerialGraph<VertexData,EdgeData>::edge_iterator>
BoostSerialGraph<VertexData,EdgeData>::edges()const{
    return boost::edges(g_);
}

template<typename VertexData,typename EdgeData>
BoostSerialGraph<VertexData,EdgeData>::node_t::node_t()
:
data(),
id(KernelConsts::invalid_size_type())
{}

template<typename VertexData,typename EdgeData>
BoostSerialGraph<VertexData,EdgeData>::node_t::node_t(const typename BoostSerialGraph<VertexData,EdgeData>::vertex_data_t& data)
:
data(data),
id(KernelConsts::invalid_size_type())
{}

template<typename VertexData,typename EdgeData>
BoostSerialGraph<VertexData,EdgeData>::node_t::node_t(typename BoostSerialGraph<VertexData,EdgeData>::vertex_data_t&& data)
:
data(data),
id(KernelConsts::invalid_size_type())
{}

template<typename VertexData,typename EdgeData>
BoostSerialGraph<VertexData,EdgeData>::node_t::node_t(const typename BoostSerialGraph<VertexData,EdgeData>::node_t& o)
:
data(o.data),
id(o.id)
{}


template<typename VertexData,typename EdgeData>
typename BoostSerialGraph<VertexData,EdgeData>::node_t&
BoostSerialGraph<VertexData,EdgeData>::node_t::operator=(const typename BoostSerialGraph<VertexData,EdgeData>::node_t& o){

    if(this==&o)
        return *this;

    this->data = o.data;
    this->id = o.id;
    return *this;

}

/*template<typename VertexData,typename EdgeData>
bool operator==(const typename boost_unidirected_serial_graph<VertexData,EdgeData>::vertex_type& v1,
                const typename boost_unidirected_serial_graph<VertexData,EdgeData>::vertex_type& v2){

    return (v2.id == v1.id);
}

template<typename VertexData,typename EdgeData>
bool operator!=(const typename boost_unidirected_serial_graph<VertexData,EdgeData>::vertex_type& v1,
                const typename boost_unidirected_serial_graph<VertexData,EdgeData>::vertex_type& v2){

    return !(v2 == v1);
}*/

/*template<typename VertexData>
inline
bool operator==(const typename boost_unidirected_serial_graph<VertexData,void>::vertex_type& v1,
                const typename boost_unidirected_serial_graph<VertexData,void>::vertex_type& v2){

    return (v2.id == v1.id);
}

template<typename VertexData>
inline
bool operator!=(const typename boost_unidirected_serial_graph<VertexData,void>::vertex_type& v1,
                const typename boost_unidirected_serial_graph<VertexData,void>::vertex_type& v2){

    return !(v2 == v1);
}*/


}

#endif // BOOST_SERIAL_GRAPH_H

#ifndef CLUSTER_H
#define CLUSTER_H

#include "cubic_engine/base/cubic_engine_types.h"
#include "kernel/maths/matrix_utilities.h"

#include <vector>
#include <algorithm>
#include <exception>

namespace cengine
{

/// \brief Simple implementation of a cluster

template<typename PointType>
struct Cluster
{
    /// \brief The type of the object representing
    /// the centroid
    typedef PointType point_t;

    /// \brief The id of the cluster
    uint_t id;

    /// \brief The centroid of the cluster
    point_t centroid;

    /// \brief Ids of the of the points that have been
    /// clustered in this cluster
    std::vector<uint_t> points;

    /// \brief Flag indicating whether the cluster changed
    bool changed;

    /// \brief Adds to this cluster the point with id pid. Returns
    /// true if and only if the given point already exists in this
    /// cluster which in this case does nothing.
    bool add_to_cluster(uint_t pid);

    /// \brief Remove from this cluster the point with the given id.
    /// Returns true if and only if the point is found. Otherwise, it returns
    /// false and does not remove anything.
    bool remove_from_cluster(uint_t pid);

    /// \brief recalculate the cluster centroid
    template<typename DataSetType>
    void recalculate_centroid(const DataSetType& set);
};


template<typename DataPoint>
bool
Cluster<DataPoint>::add_to_cluster(uint_t pid){

    typedef std::vector<uint_t>::iterator iterator;

    /// TODO: paralelize this
    iterator it = std::find_if(points.begin(),points.end(),
                               [pid](uint_t p)->bool{return (p == pid);});

    if(it !=points.end()){
        return true;
    }

    points.push_back(pid);
    changed = true;
    return false;
}


template<typename DataPoint>
bool
Cluster<DataPoint>::remove_from_cluster(uint_t id){

    typedef std::vector<uint_t>::iterator iterator;

    iterator it = std::find_if(points.begin(),points.end(),
                            [id](uint_t p)->bool{return p == id;});

    if(it !=points.end()){

        points.erase(std::remove_if(points.begin(),points.end(),
                                 [=](uint_t p)->bool{return (p == id);}));

        return true;
    }
    return false;
}

template<typename DataPoint>
template<typename DataSetType>
void
Cluster<DataPoint>::recalculate_centroid(const DataSetType& set){


    if(points.empty()){
        throw std::logic_error("Cannot calculate centroid from empty points list");
    }

    typedef typename Cluster<DataPoint>::point_t point_t;
    point_t tmp = kernel::get_row(set, points[0]);

    for(uint_t t = 1; t<points.size(); ++t){
        tmp += kernel::get_row(set, points[t]);
    }

    tmp /= points.size();
    centroid = tmp;
}

}

#endif // CLUSTER_H

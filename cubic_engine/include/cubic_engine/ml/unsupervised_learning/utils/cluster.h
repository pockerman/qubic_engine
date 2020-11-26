#ifndef CLUSTER_H
#define CLUSTER_H

#include "cubic_engine/base/cubic_engine_types.h"
#include "kernel/maths/matrix_utilities.h"
#include "kernel/utilities/common_uitls.h"

#include <vector>
#include <algorithm>
#include <exception>

namespace cengine
{

///
/// \brief Simple implementation of a cluster
///
template<typename PointType>
struct Cluster
{
	///
    /// \brief The type of the object representing
    /// the centroid
	///
    typedef PointType point_t;

	///
    /// \brief The id of the cluster
	///
    uint_t id;

	///
    /// \brief The centroid of the cluster
	///
    point_t centroid;

	///
    /// \brief Ids of the of the points that have been
    /// clustered in this cluster
	///
    std::vector<uint_t> points;

	///
    /// \brief Flag indicating whether the cluster changed
	///
    bool changed;
	
	///
	/// \brief Flag indicating if the assigned centroid is valid
	/// True by default
	///
	bool valid_centroid{true};
	
	///
	/// \brief Default constructor
	///
	Cluster()=default;
	
	///
	/// \brief Constructor
	///
	Cluster(uint_t idx, const point_t& p, const std::vector<uint_t>& pts);

    ///
    /// \brief get_id Returns the id of the cluster
    ///
    uint_t get_id()const{return id;}

    ///
    /// \brief get_points. Returns read/write reference to the
    /// points belonging to the cluster
    ///
    std::vector<uint_t>& get_points(){return points;}

    ///
    /// \brief get_points. Returns read/write reference to the
    /// points belonging to the cluster
    ///
    const std::vector<uint_t>& get_points()const{return points;}

	///
    /// \brief Adds to this cluster the point with id pid. Returns
    /// true if and only if the given point already exists in this
    /// cluster which in this case does nothing.
	///
    bool add_to_cluster(uint_t pid);

	///
    /// \brief Remove from this cluster the point with the given id.
    /// Returns true if and only if the point is found. Otherwise, it returns
    /// false and does not remove anything.
	///
    bool remove_from_cluster(uint_t pid);

	///
    /// \brief recalculate the cluster centroid
	///
    template<typename DataSetType>
    void recalculate_centroid(const DataSetType& set);
	
	///
	/// \brief Compute the intra cluster variance
	///
	template<typename DataSetType, typename Similarity>
	real_t compute_intra_variance(const DataSetType& data, 
	const Similarity& similarity)const;
	
	///
	/// \brief Copy the subset of the data contained in the cluster
	///
	template<typename DataSetType>
	void copy_subset(const DataSetType& datain, DataSetType& out)const;

    ///
    /// \brief n_points Returns the number of points in the cluster
    ///
    uint_t n_points()const{return points.size();}
	
	
};

template<typename DataPoint>
Cluster<DataPoint>::Cluster(uint_t idx, const point_t& p, const std::vector<uint_t>& pts)
:
id(idx),
centroid(p),
points(pts),
changed(false),
valid_centroid(true)
{}


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
    point_t tmp = set.get_row(points[0]);

    for(uint_t t = 1; t<points.size(); ++t){
        tmp += set.get_row(points[t]);
    }

    tmp /= points.size();
    centroid = tmp;
}

template<typename DataPoint>
template<typename DataSetType, typename Similarity>
real_t 
Cluster<DataPoint>::compute_intra_variance(const DataSetType& data, 
                                           const Similarity& similarity)const{
											   
											   
	if(points.empty()){
		throw std::logic_error("Cannot calculate cluster variance from empty points list");
	}
	
	if(!valid_centroid){
		throw std::logic_error("Centroid is not valid");
	}

	auto sum = 0.0;
	for(uint_t i=0; i<points.size(); ++i){
            auto seq = data.get_row(points[i]);
            auto dist = similarity(seq, centroid);
            sum += kernel::utils::sqr(dist);
	}
		
	return sum/points.size();
}

template<typename DataPoint>
template<typename DataSetType>
void 
Cluster<DataPoint>::copy_subset(const DataSetType& datain, DataSetType& out)const{
	
	if(points.empty()){
		throw std::logic_error("Cannot calculate cluster variance from empty points list");
	}
	
	for(uint_t i=0; i<points.size(); ++i){
				
		auto row = datain.get_row(points[i]);
		out.set_row(i, row);
	}
}

}//cengine

#endif // CLUSTER_H

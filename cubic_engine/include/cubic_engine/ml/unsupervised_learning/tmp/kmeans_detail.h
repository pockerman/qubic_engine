/* 
 * File:   kmeans_detail.h
 * Author: david
 *
 * Created on July 28, 2016, 1:10 PM
 */

#ifndef KMEANS_DETAIL_H
#define	KMEANS_DETAIL_H

#include "parml/base/parml.h"
#include "parml/base/parml_config.h"
#include "parml/base/data_adaptor.h"
#include "utilities/range1d.h"
#include "parframepp/runtime/mpi_rt_base.h"
#include "parframepp/runtime/rt_cput.h"

#ifdef PARML_DEBUG
#include "utilities/exceptions.h"
#endif

#include <limits>
#include <algorithm>

namespace parml
{
    
    namespace kmeans_detail{
        
        

        
        //forward declarations
        struct ClusterPoint;
 
#ifdef PARML_HAS_MPI
        
        /**
         * @brief Resize the given container for properly sending and receiving
         * ClusterPoint data from the participating processes. If the processor id
         * is equal to the parframepp::RTBase<mpi_par>::master_process_id() id then
         * points will have size ParML::n_processors()-1 and each array will be
         * resized  according to the induced size from the given partitions array. Otherwise
         * points will have size 1 and the array will be resized to local_size
         */
        void resize_receive_cluster_container(std::vector<std::vector<ClusterPoint> >& points,
                                              const std::vector<utilities::range1d<size_type> > partitions);        
#endif

        
        struct ClusterPoint
        {
            /**
             * @brief The Point id
             */
            size_type pid;
            
            /**
             * @brief The cluster id the point belongs to
             */
            size_type cid;
            
            
            ClusterPoint(size_type id=ParML::iuint(), 
                         size_type cluster_id=ParML::iuint())
                    :
                    pid(id),
                    cid(cluster_id)
            {}
            
            /**
              *@brief serialize the object into the given archive
            */
            template<typename Archive>
            void serialize(Archive& arch,size_type fileversion);
            
            /**
              *@brief Overload operator(). Compatible for when using boost.mpi
              *to transmit this type over the processes.
              */
            ClusterPoint operator()(const ClusterPoint& in,const ClusterPoint& out);
        };
        
        
        template<typename Archive>
        void 
        ClusterPoint::serialize(Archive& arch,size_type fileversion){
            
            arch & pid;
            arch & cid;
        }
        
        inline
        ClusterPoint 
        ClusterPoint::operator()(const ClusterPoint& in,const ClusterPoint& out){
            
            return ClusterPoint(in.pid,in.cid);
        }
        
        /**
         * @brief A small struct that represents a cluster
         */
        template<typename DataPoint>
        struct Cluster
        {
            /*
             * @brief The id of the cluster
             */
            size_type id;
            
            /**
             * @brief The location of the cluster centroid
             */
            DataPoint location;
            
            /**
             * @brief The ids of the data points that belong to this 
             * cluster
             */
            std::vector<ClusterPoint> points;
            
            /**
             * @brief A flag indicating that the cluster has changed. Default is false.
             */
            bool changed;
            
            
            /**
             * @brief Default constructor. 
             */
            Cluster()
                    :
                    id(ParML::iuint()),
                    location(),
                    points(),
                    changed(false)
            {}
            
            
            /**
             * @brief Constructor. Construct by passing the id and
             * the location of the cluster centroid
             */
            Cluster(size_type id_,DataPoint loc_)
                    :
                    id(id_),
                    location(loc_),
                    points(),
                    changed(false)
            {}
            
            /**
             * @brief Adds to this cluster the point with id pid. Returns
             * true if and only if the given point already exists in this
             * cluster which in this case does nothing.
             */
            bool add_to_cluster(size_type pid);
            
            /**
             * @brief Remove from this cluster the point with the given id.
             * Returns true if and only if the point is found. Otherwise, it returns
             * false and does not remove anything.
             */
            bool remove_from_cluster(size_type pid);
            
            
            /**
             * @brief Remove from this cluster the given data points
             */
            void remove_from_cluster(const std::vector<ClusterPoint>& pts);
            
            
            /**
             * @brief Assign the location of the cluster
             */
            void assign_location(const DataPoint& p){location = p;}
            
            /**
             * @brief Recalculates the new centroid for the cluster based on
             * the data points that have been assigned to the cluster
             * @param input
             */
            template<typename DataInput>
            void recalculate_centroid(const DataInput& input);
            
            
            /**
             * @brief Add the  given points to this cluster
             */
            void add_points(const std::vector<ClusterPoint>& pts );
        };
        
        /**
         * @brief Cluster the given point to the appropriate cluster by using the
         * given similarity function.
         */
        template<typename DataPoint,typename Similarity>
        void cluster(const DataPoint& point,size_type pid,
                     const Similarity& sim, std::vector<Cluster<DataPoint> >& clusters);
        
        
        template<typename DataPoint>
        void 
        Cluster<DataPoint>::add_points(const std::vector<ClusterPoint>& pts){
        
            if(pts.empty() || 
               pts.size() == utilities::scalar_value_traits<size_type>::zero())
                return;

            size_type current_size = points.size();
            size_type pts_size = pts.size();

            points.reserve(current_size + pts_size);

            for(size_type p=0; p<pts.size(); ++p)
                add_to_cluster(pts[p].pid); 
        }
    
        template<typename DataPoint>
        template<typename DataInput>
        void 
        Cluster<DataPoint>::recalculate_centroid(const DataInput& input){
        
            //nothing to do if this cluster has no
            //points
            if(points.empty()){
                return;
            }    

            size_type r = location.rows();
            size_type c = location.cols();
            
            //the new centroid will be the average
            //of the points in the cluster
            DataPoint avg(r,c);
        
            for(size_type p=0; p<points.size(); ++p){
            
                avg += data_adaptor::data_point(input,points[p].pid);    
            }
        
            avg /= points.size();
        
            location = avg;   
        }
    
        template<typename DataPoint>
        bool 
        Cluster<DataPoint>::add_to_cluster(size_type pid){
        
            typedef std::vector<ClusterPoint>::iterator iterator;
        
            iterator it = std::find_if(points.begin(),points.end(),
                                    [pid](const ClusterPoint& p)->bool{return (p.pid == pid);});
        
            if(it !=points.end()){
                return true;
            }
        
            points.push_back(ClusterPoint(pid,id));
            changed = true;
            return false;
           
        }
    
        template<typename DataPoint>
        void 
        Cluster<DataPoint>::remove_from_cluster(const std::vector<ClusterPoint>& pts){
        
        
            for(size_type p=0; p<pts.size(); ++p){

                remove_from_cluster(pts[p].pid);
            }
        }
    
        template<typename DataPoint>
        bool 
        Cluster<DataPoint>::remove_from_cluster(size_type id){

            typedef std::vector<ClusterPoint>::iterator iterator;

            iterator it = std::find_if(points.begin(),points.end(),
                                    [id](const ClusterPoint& p)->bool{return p.pid == id;});

            if(it !=points.end()){

                points.erase(std::remove_if(points.begin(),points.end(),
                                         [=](ClusterPoint p)->bool{return (p.pid == id);}));

                return true;  
            }
            return false;
        }
    
        template<typename DataPoint,typename Similarity>
        void cluster(const DataPoint& point,size_type pid,
                     const Similarity& sim, std::vector<Cluster<DataPoint> >& clusters){
        
                typedef typename Similarity::value_type value_type;

                value_type current_dis = std::numeric_limits<value_type>::max();
                size_type cluster_id = ParML::iuint();

                for(size_type c=0; c<clusters.size(); ++c){

                    value_type dis = sim(point,clusters[c].location);  

                    if(dis -current_dis < ParML::tolerance()){

                        current_dis = dis;
                        cluster_id = c;
                    }
                }
            
#ifdef PARML_DEBUG
            
            //make sure we have a valid cluster id
            const std::string msg = "Cluster id: "+
                                    std::to_string(cluster_id)+" not in [0,"+
                                    std::to_string(clusters.size())+")";
            Assert(cluster_id < clusters.size(),ExeInvalidClusterId(msg));           
#endif
            
            //add to the new cluster 
            //and remove the id from the others. Since each point is assigned
            //to exactly one cluster we don't have to remove from the rest
            //of the clusters if the point was found.
            
            bool found  = clusters[cluster_id].add_to_cluster(pid);
            
            if(!found){
                
                for(size_type c=0; c<clusters.size(); ++c){
                    
                    if(c!= cluster_id && !found){
                        found = clusters[c].remove_from_cluster(pid);
                    }
                }  
            }       
        }
                  
    }//kmeans_detail
        
}//parml



#endif	/* KMEANS_DETAIL_H */


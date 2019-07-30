/* 
 * File:   k_means_impl.h
 * Author: david
 *
 * Created on June 3, 2016, 7:59 AM
 */

#ifndef K_MEANS_H
#define	K_MEANS_H

#include "parml/base/parml.h"
#include "parml/base/parml_runtime.h"
#include "parml/base/data_adaptor.h"
#include "parml/base/algorithm_info.h"
#include "parml/unsupervised_learning/kmeans_detail.h"
#include "parml/unsupervised_learning/par_kmeans_detail.h"
#include "parml/unsupervised_learning/kmeans_base.h"

#include "parml/base/parml_config.h"

#include "utilities/file_writer_csv.h"
#include "utilities/scalar_value_traits.h"
#include "utilities/default_value.h"
#include "utilities/map_utilities.h"
#include "utilities/exceptions.h"
#include "utilities/range1d.h"
#include "utilities/common_functions.h"
#include "utilities/array_utilities.h"

#include "parframepp/parallel/task_base.h"
#include "parframepp/runtime/mpi_rt_base.h"
#include "parframepp/runtime/rt_cput.h"
#include "parframepp/base/parallel_indexed_object.h"

#ifdef PARML_DEBUG
#include "utilities/exceptions.h"
#endif

#include <limits>

namespace parml
{
        

template<typename RM,typename DataInput,typename CentroidType> class KMeans;
    
template<typename DataInput,typename CentroidType>
class KMeans<serial,DataInput,CentroidType>: protected kmeans_base
                                            
{
    
    public:
        
        /**
          * this runner does not modify the data given to it
          */
       typedef  const DataInput* data_input_type;
       
       /**
        * @brief Expose the type that is returned by this object
        * when calling its cluster functions
        */
        typedef KMeansInfo output_type;
       
       
       /**
        * @brief Constructor
        */ 
       KMeans(data_input_type data,const KMeansControl& kmeansdata)
               :
               kmeans_base(kmeansdata),
               data_in_(data),
               clusters_()
       {}
    
       /**
         * @brief Cluster the input data by using the given similarity
         * and initializer objects.
         * @param sim The object that computes the similarity between two data points
         * @param init The object that initializes the centroids
         */
        template<typename Similarity,typename Initializer>
        output_type cluster(const Similarity& sim,const Initializer& init);
        
        
        template<typename Initializer>
        void initialize(const Initializer& init);
        
        
        /**
         * @brief Save the clustering into a csv file
         */
        void save(const std::string& file_name)const;
        
        
    protected:
        
        /**
         * @brief The data to use
         */
        data_input_type data_in_;
         
        /**
         * @brief The clusters created
         */
        std::vector<kmeans_detail::Cluster<CentroidType> > clusters_;
                       
};

template<typename DataInput,typename CentroidType>
void 
KMeans<serial,DataInput,CentroidType>::save(const std::string& file_name)const{
    
    this->kmeans_base::save(file_name,*data_in_,clusters_);
}

template<typename DataInput,typename CentroidType>
template<typename Similarity,typename Initializer>
typename KMeans<serial,DataInput,CentroidType>::output_type 
KMeans<serial,DataInput,CentroidType>::cluster(const Similarity& sim,
                                               const Initializer& init){
    
    std::chrono::time_point<std::chrono::system_clock> start, end;
    start = std::chrono::system_clock::now();
    
    //the object that the algorithm returns
    KMeansInfo info;
    
    size_type ntotal_itrs = 0;
    
    random_restart:
    
        clusters_.clear();
        clusters_.resize(this->kmeans_base::data_.k);
        std::vector<CentroidType> centroids;
    
        this->kmeans_base::initialize_clusters(init,*data_in_,centroids,clusters_);
    
        const size_type nrows = this->kmeans_base::data_.n_training_examples;
    
        for(size_type itr=0; itr<this->kmeans_base::data_.max_n_iterations;  ++itr){

            if(this->kmeans_base::data_.show_iterations){
                std::cout<<"At iteration: "<<itr<<std::endl;
            }

            //loop over the data points
            for(size_type r=0; r<nrows; /*data_adaptor::size(*data_in_);*/ ++r){

                CentroidType p = data_adaptor::data_point(*data_in_,r);

                //cluster the data point
                kmeans_detail::cluster(p,r,sim,clusters_);
            }

            //check for empty cluster
            bool empty_cluster_found = this->kmeans_base::detect_empty_cluster(clusters_); 

            if(empty_cluster_found && 
               this->kmeans_base::data_.random_restart_on_empty_cluster){

               if(this->kmeans_base::data_.show_iterations){
                    std::cout<<"An empty cluster was found  restarting..."<<std::endl;
               }

               goto random_restart;
            }
            else if(empty_cluster_found && 
                    !this->kmeans_base::data_.continue_on_empty_cluster){

                if(this->kmeans_base::data_.show_iterations){
                    std::cout<<"An empty cluster was found  breaking..."<<std::endl;
                }
                break;
            }

            //recalculate the centroids for each cluster
            for(size_type c=0; c<clusters_.size(); ++c){

                clusters_[c].recalculate_centroid(*data_in_);
            }

            bool converged = this->kmeans_base::detect_convergence(sim,clusters_,centroids);

            if(converged){
                info.converged = converged;

                if(this->kmeans_base::data_.show_iterations){
                    std::cout<<"KMeans converged with tolerance: "<<this->kmeans_base::data_.tolerance<<std::endl;
                }

                break;   
            }

            for(size_type c=0; c<centroids.size(); ++c){
                centroids[c] = clusters_[c].location; 
            }

            ntotal_itrs++;
        }

        info.clusters.resize(clusters_.size());


        for(size_type c=0; c<clusters_.size(); ++c){
            info.clusters[c]=std::make_pair(c,clusters_[c].points.size());   
        }
        info.niterations = ntotal_itrs;
        end = std::chrono::system_clock::now();
        info.runtime = end-start;

        return info;
}


//multithreaded version of kmeans

template<typename DataInput,typename CentroidType>
class KMeans<cput_c11,DataInput,CentroidType>: protected kmeans_base,
                                               protected parframepp::par_indexed_object
{
    
    public:
        
        /**
          * this runner does not modify the data given to it
          */
       typedef  const DataInput* data_input_type;
       
       /**
        * @brief Expose the type that is returned by this object
        * when calling its cluster functions
        */
        typedef KMeansInfo output_type;
       
       /**
        * @brief Expose functions from par_indexed_object base class
        */
       using parframepp::par_indexed_object::clear_partitions;
       using parframepp::par_indexed_object::has_partitions;
       using parframepp::par_indexed_object::n_partitions;
       using parframepp::par_indexed_object::set_partitions;
       using parframepp::par_indexed_object::get_partition_id;
       using parframepp::par_indexed_object::get_global_to_local_index;
       using parframepp::par_indexed_object::get_partition_size;
       using parframepp::par_indexed_object::get_partition;
       using parframepp::par_indexed_object::get_partition_limits;
       
       /**
        * @brief Constructor
        */
       KMeans(data_input_type data,const KMeansControl& kmeansdata )
               :
               kmeans_base(kmeansdata),
               parframepp::par_indexed_object(),
               data_in_(data),
               clusters_(),
               tasks_(),
               remove_task_(nullptr)
       {}
       
       /**
        * @brief Destructor. Clean up the memory allocated for the tasks
        */
       ~KMeans(){clear_tasks();}
    
       /**
         * @brief Cluster the input data by using the given similarity
         * and initializer objects.
         * @param sim The object that computes the similarity between two data points
         * @param init The object that initializes the centroids
         */
        template<typename Similarity,typename Initializer>
        output_type cluster(const Similarity& sim,const Initializer& init);
        
        
        /**
         * @brief Save the clustering into a csv file
         */
        void save(const std::string& file_name)const;
        
        
        /**
         * @brief clears the memory allocated for the tasks.
         */
        void clear_tasks(){utilities::clear_pointer_array(tasks_);}
        
        
          
    protected:
        
        
        /**
         * @brief Merge the given clusters into the clusters container
         * @param cluster
         */
        void merge_clusters(const std::vector<kmeans_detail::Cluster<CentroidType> >& cluster);
        
        
        /**
         * @brief Merge the given clusters into the clusters container
         * @param cluster
         */
        void remove_from_rest_clusters(size_type cid,const std::vector<kmeans_detail::ClusterPoint>& points);
        
       
        /**
         * @brief The data to use
         */
        data_input_type data_in_;
        
        /**
         * @brief The clusters created
         */
        std::vector<kmeans_detail::Cluster<CentroidType> > clusters_;
        
        
        /**
         * @brief the tasks this object creates in order
         * to execute kmeans. Each task computes the distance from the
         * data points it was assigned to the centroids
         */
        std::vector<parframepp::TaskBase*> tasks_;
        
        /**
         * @brief Pointer to the task to remove data points from clusters
         */
        parframepp::TaskBase* remove_task_;
        
};



template<typename DataInput,typename CentroidType>
void 
KMeans<cput_c11,
        DataInput,
        CentroidType>::remove_from_rest_clusters(size_type cid,
                                                 const std::vector<kmeans_detail::ClusterPoint>& points){
    
    if(points.empty())
        return;
    
    for(size_type c=0; c<clusters_.size(); ++c){
        
        if(c!=cid)
            clusters_[c].remove_from_cluster(points);
    }
}

template<typename DataInput,typename CentroidType>
void 
KMeans<cput_c11,DataInput,CentroidType>::merge_clusters(const std::vector<kmeans_detail::Cluster<CentroidType> >& clusters){
    
    for(size_type c=0; c<clusters.size(); ++c){
        
        static_cast<par_kmeans_detail::RemoveTask<CentroidType>*>(remove_task_)->set_points_to_remove(clusters[c].points);
        static_cast<par_kmeans_detail::RemoveTask<CentroidType>*>(remove_task_)->set_remove_from_cluster_id(c);
        //spawned thread removes from the remaining
        //clusters the data points if they exist
        parframepp::rt_cput::schedule_task_async(remove_task_);
        
        //main thread adds the data points to the clusters
        clusters_[c].add_points(clusters[c].points);
        remove_task_->barrier();
    }
}


template<typename DataInput,typename CentroidType>
template<typename Similarity,typename Initializer>
typename KMeans<cput_c11,DataInput,CentroidType>::output_type
KMeans<cput_c11,DataInput,CentroidType>::cluster(const Similarity& sim,
                                             const Initializer& init){
    
#ifdef PARML_DEBUG
    using utilities::ExeEmptyPartitions;
    const std::string msg="The partitions have not been assigned";
    Assert(this->has_partitions()==true,ExeEmptyPartitions(msg));
#endif
    
    std::chrono::time_point<std::chrono::system_clock> start, end;
    start = std::chrono::system_clock::now();
    
    //create the tasks
    size_type nthreads = ParML::n_threads();
    
    //the object that the algorithm returns
    KMeansInfo info;
    info.nthreads = nthreads;
    
    size_type ntotal_itrs = 0;
    
    random_restart:
    
        clusters_.clear();
        clusters_.resize(this->kmeans_base::data_.k);
        std::vector<CentroidType> centroids;
    
        //initialize the clusters
        this->kmeans_base::initialize_clusters(init,*data_in_,centroids,clusters_);
    
        if(remove_task_== nullptr){
        
            remove_task_ = new par_kmeans_detail::RemoveTask<CentroidType>(clusters_);
        }
    
        size_type nparts = this->n_partitions();
    
        //resize the number of tasks
        tasks_.resize(nparts-1,nullptr);
        
        //allocate space for tasks. Keep the last task for this thread
        for(size_type t=0; t<nparts-1; ++t){
        
            tasks_[t] = new par_kmeans_detail::Task<DataInput,CentroidType,Similarity>(this->get_partition(t),*this->data_in_,sim); 
            
            //for every task created assign the centroids for its clusters   
            std::vector<kmeans_detail::Cluster<CentroidType> >& task_clusters = 
                        static_cast<par_kmeans_detail::Task<DataInput,CentroidType,Similarity>*>(tasks_[t])->get_local_clusters();
        
            task_clusters.resize(this->kmeans_base::data_.k);
            
            for(size_type c=0; c<centroids.size(); ++c){

                task_clusters[c].id = c;
                task_clusters[c].location = centroids[c];
            }
        }
       
        for(size_type itr=0; itr<this->kmeans_base::data_.max_n_iterations;  ++itr){
            
            if(this->kmeans_base::data_.show_iterations){
                std::cout<<"At iteration: "<<itr<<std::endl;
            }
        
            //schedule the tasks
            parframepp::rt_cput::schedule_tasks_async(tasks_);
        
            const utilities::range1d<size_type>& part = this->get_partition(nparts-1);
        
            size_type local_begin = part.begin(); //partitions_[nparts-1].begin();
            size_type local_end   = part.end(); //partitions_[nparts-1].end();
        
            //loop over the data points assigned for this thread
            //this is the main thread
            for(size_type r=local_begin; r<local_end; ++r){

                CentroidType p = data_adaptor::data_point(*data_in_,r);

                //cluster the data point
                kmeans_detail::cluster(p,r,sim,clusters_);
            }
        
        
            //wait for the other threads to finish and collect the 
            //local results;
            parframepp::task_barrier::barrier(tasks_);

            for(size_type t=0; t<tasks_.size(); ++t){

                std::vector<kmeans_detail::Cluster<CentroidType> >& task_clusters = 
                        static_cast<par_kmeans_detail::Task<DataInput,CentroidType,Similarity>*>(tasks_[t])->get_local_clusters();
                //get the clusters
                merge_clusters(task_clusters);
            }
        
            //check for empty cluster
            bool empty_cluster_found = this->kmeans_base::detect_empty_cluster(clusters_); 

            if(empty_cluster_found && this->kmeans_base::data_.random_restart_on_empty_cluster){
               if(this->kmeans_base::data_.show_iterations){
                    std::cout<<"An empty cluster was found  restarting..."<<std::endl;
               }
               goto random_restart;
            }
            else if(empty_cluster_found && !this->kmeans_base::data_.continue_on_empty_cluster){
                if(this->kmeans_base::data_.show_iterations){
                    std::cout<<"An empty cluster was found  breaking..."<<std::endl;
                }
                break;
            }
        
            //recalculate the centroids for each cluster
            for(size_type c=0; c<clusters_.size(); ++c){
                clusters_[c].recalculate_centroid(*data_in_);
            }
        
            bool converged = this->kmeans_base::detect_convergence(sim,clusters_,centroids);

            if(converged){
                info.converged = converged;
                if(this->kmeans_base::data_.show_iterations){
                    std::cout<<"Kmeans converged with tolerance: "<<this->kmeans_base::data_.tolerance<<std::endl;
                }
                
                break;   
            }
        
            for(size_type c=0; c<centroids.size(); ++c){
                centroids[c] = clusters_[c].location; 
            }  
        
            //reset centroids for the tasks
            for(size_type t=0; t<tasks_.size(); ++t){

                std::vector<kmeans_detail::Cluster<CentroidType> >& task_clusters = 
                        static_cast<par_kmeans_detail::Task<DataInput,CentroidType,Similarity>*>(tasks_[t])->get_local_clusters();

                for(size_type c=0; c<centroids.size(); ++c){
                    task_clusters[c].location = centroids[c];
                    task_clusters[c].points.clear();
                }

                //set the task state as pending
                tasks_[t]->set_state(parframepp::TaskBase::TaskState::PENDING);
            }
            
           ntotal_itrs++; 
        }
    
        info.clusters.resize(clusters_.size());


        for(size_type c=0; c<clusters_.size(); ++c){
            info.clusters[c]=std::make_pair(c,clusters_[c].points.size());   
        }
        info.niterations = ntotal_itrs;
        end = std::chrono::system_clock::now();
        info.runtime = end-start;

        return info;        
}

#ifdef PARML_HAS_MPI
/**
 * @brief Implementation of kmeans using MPI. Clustering will be performed
 * when the member function cluster is called. The current implementation
 * is a classical master-slave approach. The implementation is not very scalable as it assumes 
 * that the given data set perfectly fits in the memory of each participating processor.
 * The algorithm currently used is as follows. The master process initializes the clusters centroids
 * (by using the Initializer object passed to the cluster() function) which it sends to the slaved processors along
 * side with the indexes of the rows the processor must work on. Although, it is assumed that each processor has an
 * entire view of the data set to ensure consistency across possibly different machines only the master process performs 
 * the initialization of the cluster centroids. Each processor
 * then computes the distance from the cluster centroid and finds the minimum. It then
 * sends these local clusters to the master where it merges them to the current clusters it has.
 * The master determines whether convergence has been achieved  or not and notifies the slave
 * processor. 
 */
template<typename DataInput,typename CentroidType>
class KMeans<mpi_par,DataInput,CentroidType>: protected kmeans_base,
                                              protected parframepp::par_indexed_object
{
    
    public:
        
        /**
          *@brief Expose the data input type. Non const since the object must be able
          * to reset the data set according to what the master process is sending
          */
       typedef  DataInput* data_input_type;
       
       /**
        * @brief Expose the type that is returned by this object
        * when calling its cluster functions
        */
        typedef KMeansInfo output_type;
       
       /**
      * @brief Expose functions from par_indexed_object base class
      */
       using parframepp::par_indexed_object::clear_partitions;
       using parframepp::par_indexed_object::has_partitions;
       using parframepp::par_indexed_object::n_partitions;
       using parframepp::par_indexed_object::set_partitions;
       using parframepp::par_indexed_object::get_partition_id;
       using parframepp::par_indexed_object::get_global_to_local_index;
       using parframepp::par_indexed_object::get_partition_size;
       using parframepp::par_indexed_object::get_partition;
       using parframepp::par_indexed_object::get_partition_limits;
       using parframepp::par_indexed_object::get_partitions;
        
       KMeans(data_input_type data,const KMeansControl& kmeansdata )
               :
               kmeans_base(kmeansdata),
               parframepp::par_indexed_object(),
               data_in_(data),
               clusters_(),
               local_range_()
       {}
       
       /**
        * @brief Destructor. Clean up the memory allocated for the tasks
        */
       ~KMeans()
       {}
       
       /**
         * @brief Cluster the input data by using the given similarity
         * and initializer objects.
         * @param sim The object that computes the similarity between two data points
         * @param init The object that initializes the centroids
         */
        template<typename Similarity,typename Initializer>
        output_type cluster(const Similarity& sim,const Initializer& init);
        
        
        /**
         * @brief Save the clustering into a csv file. Currently,
         * only the master process will perform the output.
         */
        void save(const std::string& file_name)const;
        
        
        /**
          * @brief Save the clustering into a csv file
          */
        void save_partitions(const std::string& file_name)const;
        
        /**
         * @brief Return the local range this processor is working on
         */
        utilities::range1d<size_type>& local_range(){return local_range_;}
          
    protected:
        
        
        /**
         * @brief Merge the given clusters into the clusters container
         * @param cluster
         */
        void merge_clusters(const std::vector<std::vector<kmeans_detail::ClusterPoint> >& cluster);
        
        /**
         * @brief memory alignment of the processor clusters for  
         * sending via MPI.
         */
        void prepare_clusters_for_send(std::vector<kmeans_detail::ClusterPoint>& clusters);
        
        /**
         * @brief align The memory in the centroids into a contiguous
         * memory for transmision.
         * @param centroids
         * @param mem
         */
        template<typename T>
        void align_centroids_memory(const std::vector<CentroidType>& centroids,std::vector<T>& mem);
        
        
        /**
         * @brief The data to use
         */
        data_input_type data_in_;
        
        /**
         * @brief The clusters created
         */
        std::vector<kmeans_detail::Cluster<CentroidType> > clusters_;
        
        /**
         * @brief The local range the process is working on
         */
        utilities::range1d<size_type> local_range_;
};


template<typename DataInput,typename CentroidType>
void 
KMeans<mpi_par,DataInput,CentroidType>::save(const std::string& file_name)const{
    if(ParML::pid() == parframepp::rt_base<mpi_par>::master_process_id()){
     this->kmeans_base::save(file_name,*data_in_,clusters_); 
    }
}

template<typename DataInput,typename CentroidType>
void  
KMeans<mpi_par,DataInput,CentroidType>::save_partitions(const std::string& file_name)const{
    
   if(ParML::pid() == parframepp::rt_base<mpi_par>::master_process_id()){
       
        utilities::file_writer_csv writer(file_name);
        writer.open();
    
        std::vector<std::string> names(data_in_->cols()+1);
    
        names[0] = "PId";
    
        for(size_type n=1; n<names.size(); ++n){
            names[n] = "X"+std::to_string(n);
        }
    
        //write the names
        writer.write_header(names);
    
        std::vector<real_type> row(names.size());
        
        for(size_type p=0; p<data_in_->rows(); ++p){
            
            row[0] = this->get_partition_id(p); 
            
            const CentroidType point = data_adaptor::data_point(*data_in_,p);
            
            for(size_type col=1; col<names.size(); ++col){
               
                    row[col] = point[col-1];
            } 
            
            writer.write_row(row); 
        }
        
        //close the file
        writer.close();     
   }    
}

/*template<typename DataInput,typename CentroidType>
template<typename PartFun>
void 
KMeans<mpi_par,
       DataInput,
       CentroidType>::partition(const utilities::range1d<size_type>& range,
                                 const PartFun& partitioner){
    
    //how many threads to use
    size_type nprocs = ParML::n_processors();
    
    this->par_indexed_object<mpi_par>::partition(nprocs,range,partitioner);  
    
    //set the ranges that each process will work on
    local_range_.set_range(this->get_partition(parframepp::rt_base<mpi_par>::pid()).begin(), 
                           this->get_partition(parframepp::rt_base<mpi_par>::pid()).end());

}*/

template<typename DataInput,typename CentroidType>
void 
KMeans<mpi_par,
      DataInput,
      CentroidType>::prepare_clusters_for_send(std::vector<kmeans_detail::ClusterPoint>& clusters){

     if(ParML::pid() != parframepp::rt_base<mpi_par>::master_process_id()){
         
         size_type counter = 0;
        
         //loop over the clusters this processor has created
         for(size_type c=0; c<clusters_.size(); ++c){
             
             //loop over the points the cluster has
             for(size_type pts=0; pts<clusters_[c].points.size(); ++pts)
             
                clusters[counter++] = std::move(clusters_[c].points[pts]);    
         }
    }
}

template<typename DataInput,typename CentroidType>
template<typename T>
void 
KMeans<mpi_par,
       DataInput,
       CentroidType>::align_centroids_memory(const std::vector<CentroidType>& centroids,
                                             std::vector<T>& mem){
    
    size_type counter = 0;
    
    for(size_type i=0; i<centroids.size(); ++i){
        
        for(size_type c=0; c<centroids[i].cols(); ++c){
            
            mem[counter++] = centroids[i][c];
        }   
    }    
}

template<typename DataInput,typename CentroidType>
void 
KMeans<mpi_par,
       DataInput,
       CentroidType>::merge_clusters(const std::vector<std::vector<kmeans_detail::ClusterPoint> >& cluster){
    
    
    if(ParML::pid() == parframepp::rt_base<mpi_par>::master_process_id()){
        
        for(size_type p=0; p<cluster.size(); ++p){
            
            for(size_type pts=0; pts<cluster[p].size(); ++pts){
                
                const kmeans_detail::ClusterPoint point = cluster[p][pts];
                
                //insert into the cluster
                size_type cid = point.cid;
                
#ifdef PARML_DEBUG
            
                //make sure we have a valid cluster id
                const std::string msg = "Cluster id: "+std::to_string(cid)+" not in [0,"+
                                        std::to_string(clusters_.size())+")";
                Assert(cid < clusters_.size(),kmeans_detail::ExeInvalidClusterId(msg));           
#endif   
                
                clusters_[cid].add_to_cluster(point.pid);
            
                //remove from the other clusters
                for(size_type c=0; c<clusters_.size(); ++c){

                    if(c != cid )
                     clusters_[c].remove_from_cluster(point.pid);      
                }      
            }
        }  
    }
}

template<typename DataInput,typename CentroidType>
template<typename Similarity,typename Initializer>
typename KMeans<mpi_par,DataInput,CentroidType>::output_type 
KMeans<mpi_par,DataInput,CentroidType>::cluster(const Similarity& sim,const Initializer& init){
    

#ifdef PARML_DEBUG
    using utilities::ExeEmptyPartitions;
    const std::string msg="The partitions have not been assigned";
    Assert(partitions_.empty()==false,ExeEmptyPartitions(msg));
#endif
    
    std::chrono::time_point<std::chrono::system_clock> start, end;
    start = std::chrono::system_clock::now();
    
    //create the tasks
    size_type nthreads = ParML::n_threads();
    
    //the object that the algorithm returns
    KMeansInfo info;
    info.nthreads = nthreads;
    info.nprocs = ParML::n_processors();
    
    size_type ntotal_itrs = 0;
    
    
    random_restart:
    
        clusters_.clear();
        clusters_.resize(this->kmeans_base::data_.k);
    
        for(size_type c=0; c<clusters_.size(); ++c){
            clusters_[c].id = c;
        }
    
        std::vector<CentroidType> centroids;
    
        //the data type used to align the memory 
        //of centroids to transmit.
        std::vector<real_type> centroids_mem;
    
        //first we need to broadcast the dimension
        //of the centroid type.
        size_type centroid_dim = this->kmeans_base::data_.n_features;

    
        //initialize the clusters. Only the master process performs the initialization
        //to ensure consistency across different machines
        if(ParML::pid() == parframepp::rt_base<mpi_par>::master_process_id()){

            this->kmeans_base::initialize_clusters(init,*data_in_,centroids,clusters_);
            //centroid_dim = centroids[0].cols();    
        }
    
        //broadcast the dimensions of the centroid
        //parframepp::RTBase<mpi_par>::broadcast(centroid_dim,parframepp::RTBase<mpi_par>::master_process_id());
    
        centroids_mem.resize(this->kmeans_base::data_.k*centroid_dim);
    
        if(ParML::pid() != parframepp::rt_base<mpi_par>::master_process_id()){

            centroids.resize(this->kmeans_base::data_.k);

            for(size_type c=0; c<centroids.size(); ++c)
                centroids[c].resize(centroid_dim);    
        }
    
        size_type centroids_data_count = centroid_dim*this->kmeans_base::data_.k;
    
        align_centroids_memory(centroids,centroids_mem);
    
        //broadcast the cluster centroids computed by the master process
        parframepp::rt_base<mpi_par>::broadcast(&centroids_mem[0],centroids_data_count,
                                           parframepp::rt_base<mpi_par>::master_process_id());
    
   
        //each process now has the centroids
        if(ParML::pid() != parframepp::rt_base<mpi_par>::master_process_id()){
        
            size_type counter = 0;
        
            for(size_type c=0; c<clusters_.size(); ++c){
                clusters_[c].location.resize(centroid_dim);
                for(size_type dim=0; dim<centroid_dim; ++dim){

                    clusters_[c].location[dim] = centroids_mem[counter++];
                }
            }  
        }
    
        //for every process the master process receives a vector
        //of points that belong to every cluster
        typedef std::vector<std::vector<kmeans_detail::ClusterPoint> > receive_clusters_container_type;
    
        //the extra array used for sending/receiving the clusters
        receive_clusters_container_type receive_clusters_points;
        kmeans_detail::resize_receive_cluster_container(receive_clusters_points,this->partitions_);
    
        //the extra array for aligning in the memory the ClusterPoints struct
        //that every cluster in the process holds
        typedef std::vector<kmeans_detail::ClusterPoint> send_clusters_container_type;
    
        size_type local_size = local_range_.end() - local_range_.begin();
        send_clusters_container_type send_clusters_container;
    
        //the send is only resized if the processor is not the master
        if(ParML::pid() != parframepp::rt_base<mpi_par>::master_process_id()){

            //we will transmit local_size data points
            send_clusters_container.resize(local_size);
        }    
    
    
        //the tag used to point-to-point communicate
        //the clusters the processors created
        size_type recv_clusters_tag = 1;
  
        for(size_type itr=0; itr<this->kmeans_base::data_.max_n_iterations; ++itr){
            
            
            if(this->kmeans_base::data_.show_iterations){
                parframepp::rt_root::Info<<"At iteration: "<<itr<<std::endl;
            }
        
            size_type global_point_idx = local_range_.begin();
        
            //loop over the data points this process holds
            for(size_type r=0; r<local_range_.size(); ++r){
            
                CentroidType p = data_adaptor::data_point(*data_in_,r);
            
                //cluster the data point
                kmeans_detail::cluster(p,global_point_idx,sim,clusters_);
                global_point_idx++;
            }
        
            //prepare the clusters to send to the master process
            //only non-master processes execute this
            prepare_clusters_for_send(send_clusters_container);
        
        
            if(ParML::pid() == parframepp::rt_base<mpi_par>::master_process_id()){
            
                //do a bunch of receives
                for(size_type p=0; p<ParML::n_processors(); ++p){
                
                    if(p != parframepp::rt_base<mpi_par>::master_process_id()){
                    
                        //figure out how many points this processor is sending
                        size_type how_many = this->get_partition_size(p); //n_data_points_on_proc(p);
                    
                        //receive how_many data points from processor
                        //p and store them into receive_clusters_points[p-1][0]
                        parframepp::rt_base<mpi_par>::receive(&receive_clusters_points[p-1][0],
                                                         how_many,p,recv_clusters_tag);  
                    }
                }
             
                //after we have collected the all the clusters we must
                //merge them with the clusters of the master process
                merge_clusters(receive_clusters_points);
            }
            else{
            
                //the process is sending local_size clusters contained
                //in the send_clusters_container[0]
                parframepp::rt_base<mpi_par>::send(&send_clusters_container[0],local_size,
                                                  parframepp::rt_base<mpi_par>::master_process_id(),
                                                  recv_clusters_tag);    
            }
        
        
            int empty_cluster_signal;
            bool empty_cluster_found = false;
         
            if(ParML::pid() == parframepp::rt_base<mpi_par>::master_process_id()){
                //check for empty cluster
                empty_cluster_found = this->kmeans_base::detect_empty_cluster(clusters_);
                empty_cluster_signal = empty_cluster_found ? 1:-1;
            
            }
         
            //broadcast that there is an empty cluster
            parframepp::rt_base<mpi_par>::broadcast(empty_cluster_signal,
                                                    parframepp::rt_base<mpi_par>::master_process_id());
        
            empty_cluster_found = empty_cluster_signal==1 ? true:false;
        
            if(empty_cluster_found && 
               this->kmeans_base::data_.random_restart_on_empty_cluster){
                if(this->kmeans_base::data_.show_iterations){
                    parframepp::rt_root::Info<<"An empty cluster was found  restarting..."<<std::endl;
                }
               goto random_restart;
            }
            else if(empty_cluster_found && 
                    !this->kmeans_base::data_.continue_on_empty_cluster){
                if(this->kmeans_base::data_.show_iterations){
                    parframepp::rt_root::Info<<"An empty cluster was found  breaking..."<<std::endl;
                }
                
                break;
            }
        
            int convergence_signal;
            bool converged = false;
        
            //recalculate the centroids for each cluster. Only the master
            //process does this
            if(ParML::pid() == parframepp::rt_base<mpi_par>::master_process_id()){
            
                //recalculate the centroids
                for(size_type c=0; c<clusters_.size(); ++c){
                    clusters_[c].recalculate_centroid(*data_in_);
                }

                //check if we have converged
                converged = this->kmeans_base::detect_convergence(sim,clusters_,centroids);
                convergence_signal = converged ? 1:-1;
            }
        
            //broadcast that the algorithm converged or not
            parframepp::rt_base<mpi_par>::broadcast(convergence_signal,
                                               parframepp::rt_base<mpi_par>::master_process_id()); 
        
            converged = (convergence_signal==1 ? true:false);
        
            if(converged){
                if(this->kmeans_base::data_.show_iterations){
                    parframepp::rt_root::Info<<"Kmeans converged with tolerance: "
                                            <<this->kmeans_base::data_.tolerance<<std::endl;
                }
                break;   
            }
       
            //if we haven't converged then recalculate the centroids and
           //send them over
           if(ParML::pid() == parframepp::rt_base<mpi_par>::master_process_id()){

                for(size_type c=0; c<clusters_.size(); ++c){
                    centroids[c] = clusters_[c].location;
                }    
            }
       
            align_centroids_memory(centroids,centroids_mem);
    
            //broadcast the cluster centroids computed by the master process
            parframepp::rt_base<mpi_par>::broadcast(&centroids_mem[0],centroids_mem.size(),/*centroids_data_count,*/
                                                parframepp::rt_base<mpi_par>::master_process_id());
    
            //each process now has the centroids
            if(ParML::pid() != parframepp::rt_base<mpi_par>::master_process_id()){

                size_type counter = 0;
                for(size_type c=0; c<clusters_.size(); ++c){

                    for(size_type dim=0; dim<centroid_dim; ++dim){
                        clusters_[c].location[dim] = centroids_mem[counter++];
                    }
                }  
            }
            
            ntotal_itrs++;
        }//itrs
        
        if(ParML::pid() == parframepp::rt_base<mpi_par>::master_process_id() ){
            
            info.clusters.resize(clusters_.size());

            for(size_type c=0; c<clusters_.size(); ++c){
                info.clusters[c]=std::make_pair(c,clusters_[c].points.size());   
            }
            
            info.niterations = ntotal_itrs;
            end = std::chrono::system_clock::now();
            info.runtime = end-start;
        }

        return info;  
    
        //let's print the number of points in each cluster
        /*for(size_type c=0; c<clusters_.size(); ++c){

            parframepp::RTRoot::Info<<"Number of points in cluster: "<<c<<": "<<clusters_[c].points.size()<<std::endl;
        } */ 
}    
    
    
#endif

}//parml



#endif	/* K_MEANS_IMPL_H */


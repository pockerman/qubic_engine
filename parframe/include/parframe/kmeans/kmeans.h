#ifndef KMEANS_H
#define KMEANS_H

#include "base/types.h" //application types 

#include <vector>
#include <utility>
#include <stdexcept>
#include <chrono>
#include <memory>

namespace kmeans
{

	//Implements the KMeans algorithm
	class KMeans
	{
		
		public:

     	/// \brief An enumeration holding the types of result  
    	enum class ResultType{RESTART,BREAK, CONVERGED, CONTINUE, INVALID_TYPE};
		
			/// \brief The result that the algorithm returns
			struct ResultSet;
			typedef ResultSet result_t;

			/// \brief The configuration type
			struct Config;
			typedef Config config_t;

			// the configuration type
			struct Config
			{
				/// \brief The number of clusters
				uint_t n_clusters;

				/// \brief The number of K-Means iterations
				uint_t n_itrs; 

				/// \brief Print info
				bool print_info;

				Config()
				:
				n_clusters(0),
				n_itrs(0),
				print_info(false)
				{}

			};
	
			/// \brief Construictor. Initialize the algorithm with the given configuration 
			KMeans(const config_t&);
		
			/// \brief Cluster the points in the given dataset.
			/// Internally this function simply orchestrates the computaions 
			/// to be performed in order to cluster the points in the given dataset.
			template<typename DataSet_t, typename Similarity_t, typename Init_t>
			void cluster(const DataSet_t& dataset, const Similarity_t& sim, const Init_t& init);

			/// \brief cluster the points in the given dataset.
			/// Internally this function simply orchestrates the computations
			/// to be performed in order to cluster the points in the given dataset
			template<typename DataSet_t, typename Similarity_t>
			void cluster(const DataSet_t& dataset, const Similarity_t& sim, const std::vector<std::pair<uint_t, typename DataSet_t::point_t>>& init);

		private:

			// the configuration the algorithm uses
			config_t config_;
			
			// the result set that the algorith returns
			//result_t result_;

			/// \brief Handles the clustering task. 
			/// Given a list of data points of type DataPoint_t
			/// a list of centroids of type DataPoint_t computes the
			/// similarity between the data point and each of the
			/// centroids using the supplied similarity metric 
			template<typename Similarity_t, typename DataPoint_t>
			struct cluster_task
			{

					cluster_task(const Similarity_t& sim, std::vector<DataPoint_t>& centroids);

			};	


			/// \brief the object that actually handles the K-means iteration
			struct Iteration
			{

        /// \brief Create the tasks for this object. Upon creation it also
        /// sets the number of clusters for each created task.
       template<typename DataIn,typename Similarity>
       void create_tasks(const DataIn& data,const Similarity& sim){}


				/// \brief the pool of tasks this object is generating
        //std::vector<std::shared_ptr<TaskBase>> tasks_;
			};
	};


	template<typename Similarity_t, typename DataPoint_t>
	KMeans::cluster_task<Similarity_t, DataPoint_t>::cluster_task(const Similarity_t& sim, std::vector<DataPoint_t>& centroids)
	{}


	template<typename DataSet_t, typename Similarity_t, typename Init_t>
	void 
	KMeans::cluster(const DataSet_t& dataset, const Similarity_t& sim, const Init_t& init){

			// get the initial list of centroids
			auto centroid_ids = init(dataset, config_.n_clusters);

			cluster(dataset, sim, centroid_ids);
	}


	template<typename DataSet_t, typename Similarity_t>
	void 
	KMeans::cluster(const DataSet_t& dataset, const Similarity_t& sim, const std::vector<std::pair<uint_t, typename DataSet_t::point_t>>& init){

			//having zero number clusters in the configuration is incorrect
			uint_t n_clusters = config_.n_clusters; 

			if(n_clusters <= 0){

				std::string msg = "Incorrect number of clusters specified "+std::to_string(n_clusters)+" should be greater than zero \n";
				throw std::invalid_argument(msg);
			}

			//make sure we have as meny centroids in the initialization as those supplied
			//in the configuration

			if(init.size() != n_clusters){

				std::string msg = "Incorrect number of clusters specified " + std::to_string(init.size()) + "should be equal to " + std::to_string(n_clusters)+"\n";
				throw std::invalid_argument(msg);
			}

    
    //this->info_.nthreads = parframepp::rt_cput::n_threads();
    //this->info_.n_clustering_points = data.n_rows();
    //this->info_.residual = this->cntrl_.tolerance;
    
 		//start timing 
 		std::chrono::time_point<std::chrono::system_clock> start, end;
 		start = std::chrono::system_clock::now();
 
 random_restart:
 
 
    // the old centroids initially are those supplied by the application
    //std::vector<DataPoint> centroids(init.begin(), init.end());
 
    // initialize the clusters
    // kmeans_detail::DataOnProcInitializer::initialize(data,init,this->cntrl_.k,this->clusters_,centroids);
 
    //the exit condition
    //kmeans_detail::ExitCondition exit_cond(this->cntrl_.max_n_iterations);
    

    //the object that handles the iteration
    //kmeans_detail::Iteration<parframepp::platform_type::Type::CPUT_C11> iteration(this->cntrl_);
    
    //create the tasks
    //iteration.create_tasks(data, sim,this->get_partitions());

		//create clustering tasks
    
    //what the algorithm should do at each iteration
    KMeans::ResultType action = KMeans::ResultType::INVALID_TYPE;

		const uint_t max_itrs = config_.n_itrs;

		/*
		for(uint_t itr = 0; itr < max_itrs; ++itr){

				 if(config_.print_info){
            std::cout<<"\tK-means iteration: "<<itr<<std::endl;
        }
   
        action = iteration(data,similarity,this->clusters_, centroids);
        
        if(action == KMeansBase<DataPoint>::ActionType::RESTART){
            goto random_restart;
				}
        else if(action == KMeansBase<DataPoint>::ActionType::BREAK){
            break;
				}
        else if(action == KMeansBase<DataPoint>::ActionType::CONVERGED){
            this->info_.converged = true;
            break;   
        }
    }
    

    if(action == KMeansBase<DataPoint>::ActionType::CONVERGED){
        
        this->info_.clusters.resize(this->clusters_.size());
        for(size_type c=0; c<this->clusters_.size(); ++c){
            this->info_.clusters[c]=std::make_pair(c,this->clusters_[c].points.size());   
        }
    }
    this->info_.niterations = exit_cond.current_itr;
    end = std::chrono::system_clock::now();
    this->info_.runtime = end-start; 

		*/  
	}


}

#endif

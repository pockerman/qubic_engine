#ifndef BISECTION_K_MEANS_H
#define BISECTION_K_MEANS_H

#include "cubic_engine/base/cubic_engine_types.h"
#include "cubic_engine/ml/unsupervised_learning/utils/kmeans_info.h"
#include "cubic_engine/ml/unsupervised_learning/utils/kmeans_control.h"

namespace cengine
{
	namespace ml
	{
		
		///
		/// \brief The class BisectionKMeans.
		///
		class BisectionKMeans
		{
			
		public:
		
			///
			/// \brief The output type returned upon completion
			/// of the algorithm
			///
			typedef KMeansInfo output_t;

			///
			/// \brief The input to the algorithm
			///
			typedef KMeansConfig config_t;

			///
			/// \brief The cluster type used
			///
			typedef ClusterType cluster_t;
			
			
			///
			/// \brief The cluster type used
			///
			typedef ClusterType cluster_t;

			///
			/// \brief The centroid type
			///
			typedef typename ClusterType::point_t point_t;
			
			///
			/// \brief The result after computing
			///
			typedef std::vector<cluster_t> result_t;
		
		
			///
			/// \brief Constructor
			///
			BisectionKMeans(const config_t& config);
			
			
			///
			/// \brief Cluster the given data set
			///
			template<typename DataIn, typename Similarity>
			output_t cluster(const DataIn& data, const Similarity& similarity);
			
			
			
		};
		
		
		template<typename ClusterType>
		BisectionKMeans<ClusterType>::BisectionKMeans(const KMeansConfig& cntrl)
			:
		   control_(cntrl),
		   clusters_()
		{}


		template<typename ClusterType>
		template<typename DataIn, typename Similarity>
		typename BisectionKMeans<ClusterType>::output_t
		BisectionKMeans<ClusterType>::cluster(const DataIn& data, const Similarity& similarity){


			typedef typename BisectionKMeans<ClusterType>::cluster_t cluster_t;
			typedef typename BisectionKMeans<ClusterType>::output_t output_t;

			output_t info;
			auto k = control_.k;
			auto rows = data.rows();


			// more clusters than data does not make
			// sense
			if(k > rows){
				throw std::logic_error("Number of clusters cannot be larger than number of rows");
			}

			//start timing 
			std::chrono::time_point<std::chrono::system_clock> start, end;
			start = std::chrono::system_clock::now();
		 
		 
		 
			//the old centroids
			std::vector<typename ClusterType::point_t> centroids;

			clusters_.resize(k);
			for(uint_t c=0; c<clusters_.size(); ++c){
				clusters_[c].centroid = centroids[c];
				clusters_[c].id = c;
			}

			bool converged = false;
			real_t residual = std::numeric_limits<real_t>::max();
			bool empty_cluster  = false;
			uint_t empty_cluster_id = kernel::KernelConsts::invalid_size_type();

			while (control_.continue_iterations()) {

				if(control_.show_iterations()){

					std::cout<<"\tK-means iteration: "<<control_.get_current_iteration()<<std::endl;
				}

				for(uint_t r=0; r<data.rows(); ++r){

					// try to cluster the point
					cluster_point_(kernel::get_row(data, r), r, similarity);
				}

				// check if we have empty clusters
				auto [empty_cluster, empty_cluster_id]  = check_empty_clusters_();

				if(empty_cluster){

					if ( control_.show_iterations()){
						std::cout<<"\t\tEmpty cluster detected..."<<std::endl;
					}

					if(!control_.continue_on_empty_cluster && control_.random_restart_on_empty_cluster){

						if(control_.show_iterations()){
							std::cout<<"\t\tRestarting..."<<std::endl;
						}

						goto random_restart;
					}
					else if(!control_.continue_on_empty_cluster){
						break;
					}
					else if(control_.show_iterations()){
						std::cout<<"\t\tContinue with empty cluster detected..."<<std::endl;
					}
				}

				// clalculate new centroids
				calculate_new_centroids_(data);

				// check if we converged
				auto [converged, residual] = detect_convergence_(similarity, centroids);

				control_.update_residual(residual);

				// update the centroids also
				for(uint_t c=0; c<clusters_.size(); ++c){
					centroids[c] = clusters_[c].centroid;
				}

				if(control_.show_iterations()){

				   std::cout<<"\t\t Residual at teration: "<<residual<<std::endl;
				}
			}
		 
			auto state = control_.get_state();
			end = std::chrono::system_clock::now();

			info.runtime = end-start;
			info.nprocs = 1;
			info.nthreads = 1;
			info.converged = state.converged;
			info.residual = state.residual;
			info.tolerance = state.tolerance;
			info.niterations = state.num_iterations;
			return info;
		}

		
		
	}
}


#endif
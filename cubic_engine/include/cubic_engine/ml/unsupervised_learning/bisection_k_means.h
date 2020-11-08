#ifndef BISECTION_K_MEANS_H
#define BISECTION_K_MEANS_H

#include "cubic_engine/base/cubic_engine_types.h"
#include "cubic_engine/ml/unsupervised_learning/utils/kmeans_info.h"
#include "cubic_engine/ml/unsupervised_learning/utils/kmeans_control.h"
#include "cubic_engine/ml/unsupervised_learning/serial_kmeans.h"
#include "kernel/base/kernel_consts.h"

#include <exception>
#include <iostream>
#include <chrono>
#include <tuple>

namespace cengine
{
	namespace ml
	{
		
		///
		/// \brief The class BisectionKMeans.
		///
		template<typename ClusterType>
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
			template<typename DataIn, typename Similarity, typename Initializer>
			output_t cluster(const DataIn& data, const Similarity& similarity, 
			                 const Initializer& init);
			
		private:
		
			///
			/// \brief The algorithm control
			///
			config_t control_;
			
			
			///
			/// \brief The clusters
			///
			std::vector<cluster_t> clusters_;
			
			
			///
			/// \brief Select the cluster to split
			///
			template<typename DataIn, typename Similarity>
			cluster_t& 
			select_cluster_to_split_(const DataIn& data, 
									 const Similarity& similarity);
			
			
			///
			/// \brief Split the given cluster
			///
			template<typename DataIn, typename Similarity, typename Initializer>
			void 
			sub_cluster_(cluster_t& cluster, const DataIn& data, 
						 const Similarity& similarity, const Initializer& init);
			
			///
			/// \brief Select new centroids by considering only the 
			/// given indices list
			///
			template<typename DataIn, typename Similarity>
			std::tuple<uint_t, uint_t, real_t> 
			select_new_centroids_(const std::vector<uint_t>& indices, 
								  const DataIn& data, const Similarity& similarity);
								  
			///
			/// \brief Create the first cluster
			///
			template<typename DataIn>
			void create_first_cluster_(const DataIn& data);
			
			
		};
		
		
		template<typename ClusterType>
		BisectionKMeans<ClusterType>::BisectionKMeans(const KMeansConfig& cntrl)
			:
		   control_(cntrl),
		   clusters_()
		{}


		template<typename ClusterType>
		template<typename DataIn>
		void 
		BisectionKMeans<ClusterType>::create_first_cluster_(const DataIn& data){
		
			std::vector<uint_t> indices(data.n_rows());
			for(uint_t r=0; r<indices.size(); ++r){
				indices[r] = r;
			}
				
			ClusterType init_cluster(0, typename ClusterType::point_t(), indices);
			init_cluster.valid_centroid = false;
			clusters_.push_back(init_cluster);
		}
		
		
		template<typename ClusterType>
		template<typename DataIn, typename Similarity, typename Initializer>
		typename BisectionKMeans<ClusterType>::output_t
		BisectionKMeans<ClusterType>::cluster(const DataIn& data, const Similarity& similarity, const Initializer& init){


			typedef typename BisectionKMeans<ClusterType>::cluster_t cluster_t;
			typedef typename BisectionKMeans<ClusterType>::output_t output_t;

			output_t info;
			auto k = control_.k;
			auto rows = data.n_rows();


			// more clusters than data does not make
			// sense
			if(k > rows){
				throw std::logic_error("Number of clusters cannot be larger than number of rows");
			}

			//start timing 
			std::chrono::time_point<std::chrono::system_clock> start, end;
			start = std::chrono::system_clock::now();
			create_first_cluster_(data);
			
			
			while(clusters_.size() != k && control_.continue_iterations()){
				
				if(control_.show_iterations()){

					std::cout<<"\tBisection KMeans iteration: "<<control_.get_current_iteration()<<std::endl;
				}
				
				// select the cluster to split
				auto& cluster = select_cluster_to_split_(data, similarity);
				
				// split the cluster
				sub_cluster_(cluster, data, similarity, init);
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
		
		template<typename ClusterType>
		template<typename DataIn, typename Similarity>
		typename BisectionKMeans<ClusterType>::cluster_t& 
		BisectionKMeans<ClusterType>::select_cluster_to_split_(const DataIn& data, 
															   const Similarity& similarity){
																   
			if(clusters_.empty()){
				throw std::logic_error("Cannot select cluster because list is empty");
			}
																   
			if(clusters_.size() == 1){
				return clusters_[0];
			}

			auto cluster_idx = kernel::KernelConsts::invalid_size_type();
			auto max_var = 0.0;
			
			auto cluster_begin = clusters_.begin();
			auto cluster_end = clusters_.end();
			
			for(; cluster_begin != cluster_end; ++cluster_begin){
				
				auto& cluster = *cluster_begin;
				auto intra_var = cluster.compute_intra_variance(data, similarity);
				
				if(intra_var > max_var){
					max_var = intra_var;
					cluster_idx = cluster.id;
				}
				
			}
			
				
			if(cluster_idx == kernel::KernelConsts::invalid_size_type()){
				
				throw std::logic_error("Invalid cluster index");
			}
			
			return clusters_[cluster_idx];
		}

		
		template<typename ClusterType>
		template<typename DataIn, typename Similarity, typename Initializer>
		void 
		BisectionKMeans<ClusterType>::sub_cluster_(cluster_t& cluster, const DataIn& data, 
												    const Similarity& similarity, const Initializer& init){
														
			// current data points assigned to the cluster
			auto current_indices = cluster.points;
			
			KMeansConfig new_config(2, control_.get_max_iterations());
			KMeans<ClusterType> kmeans(new_config);
			
			// create the reduced dataset
			DataIn new_data(current_indices.size(), data.n_columns());
			
			for(uint_t i=0; i<current_indices.size(); ++i){
				
				auto row = data.get_row(current_indices[i]);
				new_data.set_row(i, row);
			}
			
			kmeans.cluster(new_data.get_storage(), similarity, init);
			
			if(clusters_.size() < control_.k){
				
				auto& clusters = kmeans.get_clusters();
				
				ClusterType  new_cluster(clusters_.size(), clusters[1].centroid, clusters[1].points);
				clusters_.push_back(new_cluster);
				cluster.centroid = clusters[0].centroid;
				cluster.points = clusters[0].points;
				
				
				
			}
			
			// select a new centroid 
			/*auto [centroid_id_1, centroid_id_2, max_distance] = select_new_centroids_(current_indices, data, similarity);
			
			std::vector<uint_t> indexes1;
			indexes1.reserve(current_indices.size());
			std::vector<uint_t> indexes2;
			indexes2.reserve(current_indices.size());
			
			auto old_centroid = cluster.centroid;
			auto centroid_changed = true;
			
			while(centroid_changed){
				
				auto cidxs_begin = current_indices.begin();
				auto cidxs_end = current_indices.end();
				
				for(; cidxs_begin != cidxs_end; ++cidxs_begin){
					
					auto point = data.get_row(*cidxs_begin);
					
					auto dist1 = similarity(point, data.get_row(centroid_id_1));
					auto dist2 = similarity(point, data.get_row(centroid_id_2));
					
					if(dist1 <= dist2){
						indexes1.push_back(*cidxs_begin);
					}
					else{
						indexes2.push_back(*cidxs_begin);
					}
				}
				
				if(cluster.valid_centroid && similarity(old_centroid, data.get_row(centroid_id_1)) < control_.get_exit_tolerance()){
					centroid_changed = false;
				}
				else{

					// update the current cluster
					// indexes and centroid
					cluster.points = indexes1;
					cluster.centroid = data.get_row(centroid_id_1);
					cluster.valid_centroid = true;
					
					

					// what happens if the centroids already exist?
					ClusterType new_cluster(clusters_.size(),
										    data.get_row(centroid_id_2), indexes2);
											
					clusters_.push_back(new_cluster);
					old_centroid = cluster.centroid;
					current_indices = indexes1;
					
					indexes1 = std::vector<uint_t>();
					indexes1.reserve(current_indices.size());
					indexes2 = std::vector<uint_t>();
					indexes2.reserve(current_indices.size());
				
				}
			}*/
														
		}
		
		template<typename ClusterType>
		template<typename DataIn, typename Similarity>
		std::tuple<uint_t, uint_t, real_t> 
		BisectionKMeans<ClusterType>::select_new_centroids_(const std::vector<uint_t>& indices, 
															const DataIn& data, const Similarity& similarity){
		
			return std::make_tuple(0, 0, 0.0);
		}
		
	}// ml
}// cengine


#endif
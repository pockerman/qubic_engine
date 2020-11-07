#ifndef SOFT_LABEL_KNN_CLASSIFIER_H
#define SOFT_LABEL_KNN_CLASSIFIER_H

#include "cubic_engine/base/cubic_engine_types.h"
#include "cubic_engine/ml/instance_learning/utils/knn_control.h"
#include <boost/noncopyable.hpp>

namespace cengine
{
	namespace ml
	{
		
		template<typename DataSetType, typename LabelType, typename Similarity,
		class SoftKNN: private boost::noncopyable
		{
		public:
		
			/// \brief
			typedef KnnInfo output_t;

			///
			/// \brief The return type
			///
			typedef typename Actor::return_t return_t;

			///
			/// \brief Constructor
			///
			SoftKNN(const KnnControl& control);
			
			
			///
			/// \brief Train the model
			///
			void train(const DataSetType& data_set, const LabelType& labels);
			
			
		private:
		
			///
			/// \brief The data set over which the classifier is trainned
			///
			const DataSetType* data_ptr_;
			
			///
			/// \brief Soft labels
			///
			const LabelType* labels_ptr_;
			
			
			
		};
	}
}

#endif
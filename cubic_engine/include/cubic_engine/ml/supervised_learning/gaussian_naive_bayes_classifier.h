#ifndef GAUSSIAN_NAIVE_BAYES_CLASSIFIER_H
#define GAUSSIAN_NAIVE_BAYES_CLASSIFIER_H

#include "cubic_engine/base/cubic_engine_types.h"
#include "cubic_engine/ml/supervised_learning/naive_bayes_classifier_base.h"
#include "kernel/base/kernel_consts.h"
#include "kernel/maths/matrix_traits.h"
#include "kernel/maths/statistics/normal_distribution.h"

#include <map>
#include <utility>
#include <cmath>
#include <random>
#include <stdexcept>

namespace cengine {
namespace ml{

///
/// \brief The GaussianNBC class. Gaussina Naive Bayes
/// classifer
///
template<typename DataSetTp, typename LabelsTp>
class GaussianNBC: public NaiveBayesBase<DataSetTp, LabelsTp>
{
public:

    typedef typename NaiveBayesBase<DataSetTp, LabelsTp>::output_t output_t;
    typedef typename NaiveBayesBase<DataSetTp, LabelsTp>::labels_t labels_t;
    typedef typename NaiveBayesBase<DataSetTp, LabelsTp>::dataset_t dataset_t;

    ///
    /// \brief MultinomialNBC. Constructor
    ///
    GaussianNBC(const std::map<uint_t, std::pair<real_t, real_t>>& params);

    ///
    /// \brief train. Train the algorithm
    ///
    void train(const dataset_t& examples, const labels_t& labels);

    ///
    /// \brief Predict the class for the given data point
    ///
    template<typename DataPointTp>
    output_t predict(const DataPointTp& point)const;

    ///
    /// \brief Predict on the data set
    ///
    template<typename OutputStorageTp>
    void predict(const dataset_t& point, OutputStorageTp& out)const;

    ///
    /// \brief Returns the probability that the
    /// given data point belongs to the given class.
    ///
    template<typename DataPointTp>
    real_t get_data_point_class_probability(const DataPointTp& data_point,
                                            uint_t cls)const;


private:

    ///
    /// \brief params_ For each class the mean and
    /// standard deviation
    ///
   std::map<uint_t, std::pair<real_t, real_t>> params_;

};

template<typename DataSetTp, typename LabelsTp>
GaussianNBC<DataSetTp, LabelsTp>::GaussianNBC(const std::map<uint_t, std::pair<real_t, real_t>>& params)
    :
    NaiveBayesBase<DataSetTp, LabelsTp>(),
    params_(params)
{}


template<typename DataSetTp, typename LabelsTp>
template<typename DataPointTp>
typename GaussianNBC<DataSetTp, LabelsTp>::output_t
GaussianNBC<DataSetTp, LabelsTp>::predict(const DataPointTp& point)const{

    if(!this->examples_ptr_){
        throw std::logic_error("Train set has not been set. Did you call train?");
    }

    if(!this->labels_ptr_){
        throw std::logic_error("Labels set has not been set. Did you call train?");
    }

    //we loop over the classes and attempt to
    //calculate the probability that the data point
    //belongs to a given class the class with the maximum probability
    //along with the probability value is returned

    real_t max_prob = 0.0;
    uint_t pclassidx = kernel::KernelConsts::invalid_size_type();
    typedef std::map<uint_t, uint_t>::const_iterator class_iterator;

    class_iterator classb = params_.begin();
    class_iterator classe = params_.end();

    while(classb != classe){

        uint_t classidx = classb->first;
        real_t prob = get_data_point_class_probability(point, classidx);

        if((prob - max_prob) > kernel::KernelConsts::tolerance()){

            max_prob = prob;
            pclassidx = classidx;
        }

        classb++;
    }

  return {pclassidx, max_prob};
}


template<typename DataSetTp, typename LabelsTp>
template<typename OutputStorageTp>
void
GaussianNBC<DataSetTp, LabelsTp>::predict(const dataset_t& examples, OutputStorageTp& out)const{

    if(out.size() != examples.rows()){
        throw std::logic_error("Invalid storage size. Size should be " +
                               std::to_string(examples.rows()) +
                               " but is " + std::to_string(out.size()));
    }

    for(uint_t exe=0; exe<examples.rows(); ++ exe){

        auto example = kernel::matrix_row_trait<dataset_t>::get_row(examples, exe);
        out[exe] = predict(example);
    }
}

template<typename DataSetTp, typename LabelsTp>
template<typename DataPointTp>
real_t
GaussianNBC<DataSetTp, LabelsTp>::get_data_point_class_probability(const DataPointTp& data_point,
                                                                    uint_t cls)const{

    if(!this->examples_ptr_){
        throw std::logic_error("Train set has not been set. Did you call train?");
    }

    if(!this->labels_ptr_){
        throw std::logic_error("Labels set has not been set. Did you call train?");
    }

    if(data_point.size() != this->examples_ptr_->columns()){
        throw std::logic_error("Invalid point size. " +
                               std::to_string(data_point.size()) +
                               " not equal to: "+
                               std::to_string(this->examples_ptr_->columns()));
    }

    auto itr = params_.find(cls);

    if(itr == params_.end()){
        throw std::logic_error("Invalid class index. Class index" +
                               std::to_string(cls) +
                               " does not exist");
    }

    //loop over the features of test example
    //find the probability that the feature has for the given class
    real_t total_prob = 0.0;

    auto mu_cls = itr->second.first;
    auto std_cls = itr->second.second;

    kernel::maths::stats::NormalDistribution<real_t> dist(mu_cls, std_cls);

    for(uint_t f=0; f<data_point.size(); ++f){

        //get the feature value
        auto feature_val = data_point[f];

        total_prob += dist.pdf(feature_val);
    }

    total_prob += std::log(this->get_class_probability(cls));
    return total_prob;
}



}
}

#endif // GAUSSIAN_NAIVE_BAYES_CLASSIFIER_H

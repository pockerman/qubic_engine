#ifndef MULTINOMIAL_NAIVE_BAYES_CLASSIFIER_H
#define MULTINOMIAL_NAIVE_BAYES_CLASSIFIER_H

#include "cubic_engine/base/cubic_engine_types.h"
#include "cubic_engine/ml/supervised_learning/naive_bayes_classifier_base.h"
#include "kernel/base/kernel_consts.h"
#include "kernel/maths/matrix_traits.h"


#include <map>
#include <utility>
#include <cmath>
#include <stdexcept>

namespace cengine{
namespace ml {

///
/// \brief The MultinomialNBC class. Multinomial Naive Bayes
/// classifer
///
template<typename DataSetTp, typename LabelsTp>
class MultinomialNBC: public NaiveBayesBase<DataSetTp, LabelsTp>
{

public:

    typedef typename NaiveBayesBase<DataSetTp, LabelsTp>::output_t output_t;
    typedef typename NaiveBayesBase<DataSetTp, LabelsTp>::labels_t labels_t;
    typedef typename NaiveBayesBase<DataSetTp, LabelsTp>::dataset_t dataset_t;

    ///
    /// \brief MultinomialNBC. Constructor
    ///
    MultinomialNBC(real_t alpha=1.0);

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

    ///
    /// \brief Returns the number of training examples that
    /// the feature at index featureidx has value val and the
    /// example is classified with cls
    ///
    template<typename FeatureTp>
    uint_t get_class_n_training_examples_with_feature_val(uint_t cls,
                                                          uint_t featureidx, const FeatureTp& val)const;

private:

    ///
    /// \brief alpha_ Smoothing parameter.
    /// Default is 1.0 resulting to Laplace smoothing
    ///
    real_t alpha_;

};


template<typename DataSetTp, typename LabelsTp>
MultinomialNBC<DataSetTp, LabelsTp>::MultinomialNBC(real_t alpha)
    :
      NaiveBayesBase<DataSetTp, LabelsTp>(),
      alpha_(alpha)

{}


template<typename DataSetTp, typename LabelsTp>
template<typename FeatureTp>
uint_t
MultinomialNBC<DataSetTp, LabelsTp>::get_class_n_training_examples_with_feature_val(uint_t cls,
                                                                                    uint_t featureidx,
                                                                                    const FeatureTp& val)const{

    if(!this->examples_ptr_){
        throw std::logic_error("Train set has not been set. Did you call train?");
    }

    if(!this->labels_ptr_){
        throw std::logic_error("Labels set has not been set. Did you call train?");
    }

    if(!this->data_set_has_class(cls))
        return 0;

    if(featureidx >= this->examples_ptr_->columns() ){
        throw std::logic_error("Invalid feature index. Index not in [0, " +
                               std::to_string(this->examples_ptr_->columns()) + ")");
    }

    auto counter = 0;
    for(uint_t r=0; r<this->examples_ptr_->rows(); ++r){

        if((*this->labels_ptr_)[r] == cls){

            if((*this->examples_ptr_)(r,featureidx) == val){
                counter++;
            }
        }
    }

   return counter;
}

template<typename DataSetTp, typename LabelsTp>
template<typename DataPointTp>
real_t
MultinomialNBC<DataSetTp, LabelsTp>::get_data_point_class_probability(const DataPointTp& data_point,
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

    // loop over all features in the point
    // and calculate p(c | x_i)
    // this is given as (Nyi + alpha)/(Ny + alpha*n)

    auto ny = 0.0;
    std::vector<uint_t> nyis(data_point.size(), 0);
    for(uint_t f=0; f<data_point.size(); ++f){

        //get the feature value
        auto feature_val = data_point[f];

        //how many times the feature with the given value
        //and the given class appears in the data set
        nyis[f] = get_class_n_training_examples_with_feature_val(cls,f,feature_val);
        ny += nyis[f];
    }

    //loop over the features of test example
    //find the probability that the feature has for the given class
    real_t total_prob = 0.0;
    std::for_each(nyis.begin(), nyis.end(),
                  [&, this](uint_t nyi){total_prob += (nyi + alpha_)/(ny + alpha_ * data_point.size());});

    total_prob += std::log(this->get_class_probability(cls));
    return total_prob;
}

template<typename DataSetTp, typename LabelsTp>
template<typename DataPointTp>
typename MultinomialNBC<DataSetTp, LabelsTp>::output_t
MultinomialNBC<DataSetTp, LabelsTp>::predict(const DataPointTp& point)const{

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

    class_iterator classb = this->classes_counters_.begin();
    class_iterator classe = this->classes_counters_.end();

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
MultinomialNBC<DataSetTp, LabelsTp>::predict(const dataset_t& examples, OutputStorageTp& out)const{

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


}
}

#endif // MULTINOMIAL_NAIVE_BAYES_CLASSIFIER_H

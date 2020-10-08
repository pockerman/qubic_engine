#ifndef MULTINOMIAL_NAIVE_BAYES_CLASSIFIER_H
#define MULTINOMIAL_NAIVE_BAYES_CLASSIFIER_H

#include "cubic_engine/base/cubic_engine_types.h"
#include "kernel/base/kernel_consts.h"

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
class MultinomialNBC
{

public:

    typedef std::pair<uint_t, real_t> output_t;
    typedef LabelsTp labels_t;
    typedef DataSetTp dataset_t;

    ///
    /// \brief count_classes. Count the classes in the given
    /// labels container
    ///
    template<typename OtherLabelsTp>
    static void count_classes(std::map<uint_t, uint_t>& classes_counters,
                              const OtherLabelsTp& labels);

    ///
    /// \brief MultinomialNBC. Constructor
    ///
    MultinomialNBC();

    ///
    /// \brief train. Train the algorithm
    ///
    void train(const dataset_t& examples, const labels_t& labels)const;

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
    /// \brief get_class_probability. Returns the probability
    /// of the class with cls_idx
    ///
    real_t get_class_probability(uint_t cls_idx, uint_t rows)const;

    ///
    /// \brief Returns the probability that the
    /// given data point belongs to the given class.
    ///
    template<typename DataPointTp>
    real_t get_data_point_class_probability(const DataPointTp& data_point,
                                            uint_t cls)const;

    ///
    ///
    ///
    template<typename FeatureTp>
    uint_t get_class_n_training_examples_with_feature_val(uint_t cls,
                                                          uint_t featureidx, const FeatureTp& val)const;

    ///
    /// \brief data_set_has_class Returns true if the class index exist
    /// in the supplied labels
    ///
    bool data_set_has_class(uint_t cls)const;

private:

    ///
    /// \brief class_occurence_ A map that keeps the number of classes
    /// in the dataset and how many times this class occurs
    ///
    std::map<uint_t, uint_t> classes_counters_;

    ///
    /// \brief labels_. Pointer to the labels
    ///
    const labels_t* labels_ptr_;

    ///
    /// \brief examples_ptr_. Pointer to the examples
    ///
    const dataset_t* examples_ptr_;

    ///
    /// \brief alpha_ Smoothing parameter.
    /// Default is 1.0 resulting to Laplace smoothing
    ///
    real_t alpha_;

};

template<typename DataSetTp, typename LabelsTp>
template<typename OtherLabelsTp>
void
MultinomialNBC<DataSetTp, LabelsTp>::count_classes(std::map<uint_t, uint_t>& classes_counters,
                                                    const OtherLabelsTp& labels){

    if(labels.empty()){
        return;
    }

    for(uint_t idx=0; idx < labels.size(); ++idx){
        auto item = labels[idx];
        auto itr = classes_counters.find(item);

        if( itr != classes_counters.end()){
            itr->second += 1;
        }
        else{
           classes_counters.insert_or_assign(item, 1);
        }
    }
}

template<typename DataSetTp, typename LabelsTp>
MultinomialNBC<DataSetTp, LabelsTp>::MultinomialNBC()
    :
      classes_counters_(),
      labels_ptr_(nullptr),
      examples_ptr_(nullptr),
      alpha_(1.0)

{}

template<typename DataSetTp, typename LabelsTp>
void
MultinomialNBC<DataSetTp, LabelsTp>::train(const DataSetTp& examples,
                                           const LabelsTp& labels)const{

    // clear any occurences
    classes_counters_.empty();
    MultinomialNBC::count_classes(classes_counters_, labels);
    labels_ptr_ = &labels;
    examples_ptr_ = &examples;

}

template<typename DataSetTp, typename LabelsTp>
bool
MultinomialNBC<DataSetTp, LabelsTp>::data_set_has_class(uint_t cls)const{

    if(classes_counters_.empty()){
        throw std::logic_error("Class occurence has not been computed");
    }

    auto it = classes_counters_.find(cls);
    return it != classes_counters_.end();
}

template<typename DataSetTp, typename LabelsTp>
real_t
MultinomialNBC<DataSetTp, LabelsTp>::get_class_probability(uint_t cls_idx, uint_t rows)const{

    if(classes_counters_.empty()){
        throw std::logic_error("Class occurence has not been computed");
    }

    auto itr = classes_counters_.find(cls_idx);

    if(itr != classes_counters_.end()){

        auto counter = itr->second;
        return counter/static_cast<real_t>(rows);
    }

    return 0.0;
}

template<typename DataSetTp, typename LabelsTp>
template<typename FeatureTp>
uint_t
MultinomialNBC<DataSetTp, LabelsTp>::get_class_n_training_examples_with_feature_val(uint_t cls,
                                                                                    uint_t featureidx,
                                                                                    const FeatureTp& val)const{

    if(!examples_ptr_){
        throw std::logic_error("Train set has not been set. Did you call train?");
    }

    if(!labels_ptr_){
        throw std::logic_error("Labels set has not been set. Did you call train?");
    }

    if(!data_set_has_class(cls))
        return 0;

    if(featureidx >= examples_ptr_->columns() ){
        throw std::logic_error("Invalid feature index. Index not in [0, " +
                               std::to_string(examples_ptr_->columns()) + ")");
    }

    auto counter = 0;
    for(uint_t r=0; r<examples_ptr_->rows(); ++r){

        if((*labels_ptr_)[r] == cls){

            if((*examples_ptr_)(r,featureidx) == val){
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

    if(!examples_ptr_){
        throw std::logic_error("Train set has not been set. Did you call train?");
    }

    if(!labels_ptr_){
        throw std::logic_error("Labels set has not been set. Did you call train?");
    }


    if(data_point.size() != examples_ptr_->columns()){
        throw std::logic_error("Invalid point size. " +
                               std::to_string(data_point.size()) +
                               " not equal to: "+
                               std::to_string(examples_ptr_->columns()));
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

    total_prob *= std::log(get_class_probability(cls));
    return total_prob;
}

template<typename DataSetTp, typename LabelsTp>
template<typename DataPointTp>
typename MultinomialNBC<DataSetTp, LabelsTp>::output_t
MultinomialNBC<DataSetTp, LabelsTp>::predict(const DataPointTp& point)const{

    if(!examples_ptr_){
        throw std::logic_error("Train set has not been set. Did you call train?");
    }

    if(!labels_ptr_){
        throw std::logic_error("Labels set has not been set. Did you call train?");
    }

    //we loop over the classes and attempt to
    //calculate the probability that the data point
    //belongs to a given class the class with the maximum probability
    //along with the probability value is returned

    real_t max_prob = 0.0;
    uint_t pclassidx = kernel::KernelConsts::invalid_size_type();
    typedef std::map<uint_t, uint_t>::const_iterator class_iterator;

    class_iterator classb = classes_counters_.begin();
    class_iterator classe = classes_counters_.end();

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

}


}
}

#endif // MULTINOMIAL_NAIVE_BAYES_CLASSIFIER_H

#ifndef NAIVE_BAYES_CLASSIFIER_BASE_H
#define NAIVE_BAYES_CLASSIFIER_BASE_H

#include "cubic_engine/base/cubic_engine_types.h"

#include <map>
#include <utility>

namespace cengine {
namespace ml{

template<typename DataSetTp, typename LabelsTp>
class NaiveBayesBase
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
    /// \brief ~NaiveBayesBase Destructor
    ///
    virtual ~NaiveBayesBase(){}

    ///
    /// \brief train. Train the algorithm
    ///
    virtual void train(const dataset_t& examples, const labels_t& labels);

    ///
    /// \brief get_class_probability. Returns the probability
    /// of the class with cls_idx
    ///
    real_t get_class_probability(uint_t cls_idx)const;

    ///
    /// \brief data_set_has_class Returns true if the class index exist
    /// in the supplied labels
    ///
    bool data_set_has_class(uint_t cls)const;

protected:

    ///
    /// \brief NaiveBayesBase Constructor
    ///
    NaiveBayesBase();

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

};


template<typename DataSetTp, typename LabelsTp>
template<typename OtherLabelsTp>
void
NaiveBayesBase<DataSetTp, LabelsTp>::count_classes(std::map<uint_t, uint_t>& classes_counters,
                                                    const OtherLabelsTp& labels){

    if(labels.size() == 0){
       throw std::logic_error("Empty labels vector??? Labels size is zero");
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
NaiveBayesBase<DataSetTp, LabelsTp>::NaiveBayesBase()
    :
      classes_counters_(),
      labels_ptr_(nullptr),
      examples_ptr_(nullptr)

{}

template<typename DataSetTp, typename LabelsTp>
void
NaiveBayesBase<DataSetTp, LabelsTp>::train(const DataSetTp& examples,
                                           const LabelsTp& labels){

    // clear any occurences
    classes_counters_.empty();
    NaiveBayesBase::count_classes(classes_counters_, labels);
    labels_ptr_ = &labels;
    examples_ptr_ = &examples;

}

template<typename DataSetTp, typename LabelsTp>
real_t
NaiveBayesBase<DataSetTp, LabelsTp>::get_class_probability(uint_t cls_idx)const{

    if(classes_counters_.empty()){
        throw std::logic_error("Class occurence has not been computed");
    }

    auto itr = classes_counters_.find(cls_idx);

    if(itr != classes_counters_.end()){

        auto counter = itr->second;
        return counter/static_cast<real_t>(examples_ptr_->rows());
    }

    return 0.0;
}

template<typename DataSetTp, typename LabelsTp>
bool
NaiveBayesBase<DataSetTp, LabelsTp>::data_set_has_class(uint_t cls)const{

    if(classes_counters_.empty()){
        throw std::logic_error("Class occurence has not been computed");
    }

    auto it = classes_counters_.find(cls);
    return it != classes_counters_.end();
}



}

}

#endif // NAIVE_BAYES_CLASSIFIER_BASE_H

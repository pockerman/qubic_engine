#ifndef CONFUSION_MATRIX_H
#define CONFUSION_MATRIX_H

#include "cubic_engine/base/cubic_engine_types.h"

#include <map>
#include <vector>
#include <exception>
#include <ostream>
#include <iostream>

namespace cengine{
namespace ml {


///
/// \brief ConfusionMatrix. Wraps the idea of a confusion matrix
/// A confusion matrix can be used to evaluate the accuracy of a classification.
/// see https://en.wikipedia.org/wiki/Confusion_matrix
/// The format of the matrix is the following
/// actual classes are aligned row-wise
/// predicted classes are aligned column-wise
///
class ConfusionMatrix
{
public:

    ///
    /// \brief Costructor
    ///
    template<typename Container>
    ConfusionMatrix(const Container& actual, const Container& predicted, uint_t n_classes);

    ///
    /// \brief Construct the matrix from the given map
    ///
    template<typename Container>
    void build_from(const Container& actual, const Container& predicted, uint_t n_classes);

    ///
    /// \brief Returns the accuracy of the classifier
    ///
    real_t accuracy()const;

    ///
    /// \brief Returns the miclassification rate
    ///
    real_t misclassification_rate()const;

    ///
    /// \brief Recall also known as sensitivity or true positive rate
    /// for class c
    ///
    real_t recall_class(uint_t c)const;

    /// \breif Return the true positives. This is simply
    /// the sum of the diagonal elements
    uint_t true_positives()const;

    ///
    /// \brief Get the class counts
    ///
    uint_t get_class_counts(uint_t c)const;

    ///
    /// \brief Get class incorrect counts
    ///
    uint_t get_class_incorrect_counts(uint_t c)const;

    ///
    /// \brief Returns the counts that class c was predicted as other class
    ///
    uint_t get_class_counts_as_other_class(u_int c, uint_t other)const;

    ///
    /// \brief Returns the total number of observations
    ///
    uint_t total_count()const{return total_count_;}

    ///
    /// \brief Print the matrix
    ///
    std::ostream& print(std::ostream& out)const;

private:


    ///
    /// \brief The matrix that holds the predictions
    ///
    DynMat<uint_t> matrix_;

    ///
    /// \brief Total number of observations
    ///
    uint_t total_count_;

};

template<typename Container>
ConfusionMatrix::ConfusionMatrix(const Container& actual,
                                 const Container& predicted, uint_t n_classes)
    :
    matrix_(),
    total_count_(0)
{
    build_from(actual, predicted, n_classes);
}

template<typename Container>
void
ConfusionMatrix::build_from(const Container& actual, const Container& predicted, uint_t n_classes){

    if(n_classes <= 1){

       throw std::invalid_argument("Invalid number of classes: "+
                                   std::to_string(n_classes)+
                                   " number of classes should be at least 2");
    }

    if(actual.size() == 0){
        throw std::invalid_argument("Cannot construct ConfusionMatrix from empty data");
    }

    if(actual.size() != predicted.size()){
        throw std::invalid_argument("A ConfusionMatrix is a square matrix but the given data are: "+
                                    std::to_string(actual.size())+
                                    " and columns: "+
                                    std::to_string(predicted.size()));
    }

    matrix_.resize(n_classes, n_classes);

    for(uint_t r=0; r<matrix_.rows(); ++r){

        for(uint_t c=0; c<matrix_.rows(); ++c){
            matrix_(r,c) = 0;
        }
    }

    // construct the matrix row-wise
    for(uint_t r=0; r<actual.size(); ++r){

        uint_t row_idx = actual[r];

        if(actual[r] == predicted[r]){
           matrix_(row_idx ,row_idx) += 1;
        }
        else{
           matrix_(row_idx , predicted[r]) += 1;
        }
    }

    // set the total count of observations
    total_count_ = actual.size();

}

inline
std::ostream& operator<<(std::ostream& out, const ConfusionMatrix& mat){
    return mat.print(out);
}

}
}
#endif // CONFUSION_MATRIX_H

#ifndef CONFUSION_MATRIX_H
#define CONFUSION_MATRIX_H

#include "cubic_engine/base/cubic_engine_types.h"
#include <map>
#include <vector>

namespace cengine
{

/// \brief ConfusionMatrix. Wraps the idea of a confusion matrix
/// A confusion matrix can be used to evaluate the accuracy of a classification.
/// see https://en.wikipedia.org/wiki/Confusion_matrix
class ConfusionMatrix
{
public:

    /// \brief Costructor
    ConfusionMatrix(const std::map<uint_t, std::vector<uint_t>>& data);

    /// \brief Construct the mtrix from the given map
    void build_from(const std::map<uint_t, std::vector<uint_t>>& data);

    /// \brief recall also known as sensitivity
    real_t recall()const;

    /// \brief Return the  F1-score
    real_t f1_score()const;

    /// \breif Return the true positives. This is simply
    /// the sum of the diagonal elements
    uint_t true_positives()const;


private:


    /// \brief The matrix that holds the predictions
    DynMat<uint_t> matrix_;

};

}
#endif // CONFUSION_MATRIX_H

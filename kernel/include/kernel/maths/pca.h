#ifndef PCA_H
#define PCA_H

#include "kernel/base/types.h"
#include "boost/noncopyable.hpp"

namespace kernel{

///
/// \brief The PCA class. Principal Component Analisys
/// using SVD. Decomposes a DynMatrix<real_t> using SVD.
///
class PCA: private boost::noncopyable
{
public:

    ///
    /// \brief PCA Constructor
    /// \param ncomponents The number of components to
    /// retain from the initial data. If ncomponents=0
    /// then all components are retained
    ///
    PCA(uint_t ncomponents=0);

    ///
    /// \brief fit Apply PCA to the given data
    ///
    void fit(DynMat<real_t>& data);

    ///
    /// \brief reinitialize. Reinitialize the internal data structures.
    /// The object is brought to the same state that the occurs when
    /// the constructor is called
    ///
    void reinitialize(uint_t ncomponents);

private:

    ///
    /// \brief n_components_ How many components to keep
    ///
    uint_t n_components_;

    ///
    /// \brief total_data_var_ The total variance of the data set
    ///
    real_t total_data_var_;

    ///
    /// \brief explained_variance_.
    /// The amount of variance explained by each of the selected components.
    /// This is equal to n_components_ largest eigenvalues of the covariance matrix
    /// of the given data set
    ///
    DynVec<real_t> explained_variance_;

    ///
    /// \brief U_ Matrix with the left eigenvectors
    ///
    DynMat<real_t> U_;

    ///
    /// \brief s_ Holds the singular values
    ///
    DynVec<real_t> s_;

    ///
    /// \brief V_. Matrix with the right eigenvectors
    ///
    DynMat<real_t> V_;

};

}

#endif // PCA_H

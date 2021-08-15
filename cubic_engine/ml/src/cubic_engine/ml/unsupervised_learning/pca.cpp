#include "cubic_engine/ml/unsupervised_learning/pca.h"
#include "kernel/maths/matrix_utilities.h"
#include <stdexcept>
#include <iostream>

namespace cengine{
namespace ml{

PCA::PCA(uint_t ncomponents)
    :
    n_components_(ncomponents),
    U_(),
    s_(),
    V_()
{}

void
PCA::reinitialize(uint_t ncomponents){

    n_components_ = ncomponents;
    total_data_var_ = 0.0;

    U_.clear();
    s_.clear();
    V_.clear();
}

void
PCA::fit(DynMat<real_t>& data){

    if(n_components_ > data.columns()){
        throw std::logic_error("Number of components cannot be larger than the number of features");
    }

    // all the features are requested
    if(n_components_ == 0){
        n_components_ = data.columns();
    }

    // calculate total data variance
    total_data_var_ = 0.0;

    /*for(uint_t c=0; c<data.columns(); ++c){
        total_data_var_ += var(kernel::get_column(data, c));
    }*/

    U_.clear();
    s_.clear();
    V_.clear();

    // do SVD
    DynMat<real_t> V;
    blaze::svd(data, U_, s_, V );

    if(n_components_ == data.columns()){
        V_ = std::move(V);

        explained_variance_.resize(n_components_, 0.0);

        for(uint_t c = 0; c<s_.size(); ++c){
            total_data_var_ += (s_[c]*s_[c]);
        }
    }
    else{

        V_.resize(data.columns(), n_components_);
        explained_variance_.resize(n_components_, 0.0);

        for(uint_t c=0; c<s_.size(); ++c){
            total_data_var_ += (s_[c]*s_[c]);
        }

        for(uint_t c=0; c<n_components_; ++c){

            explained_variance_[c] = (s_[c]*s_[c])/total_data_var_;
            auto col = kernel::get_column(V, c);

            for(uint_t r=0; r<V_.rows(); ++r){
                V_(r,c) = col[r];
            }
        }

    }

    // compute the transformed data
    data = data*V_;

}
}
}

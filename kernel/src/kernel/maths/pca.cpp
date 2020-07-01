#include "kernel/maths/pca.h"
#include "kernel/maths/matrix_utilities.h"
#include <stdexcept>
namespace kernel{

PCA::PCA(uint_t ncomponents)
    :
    n_components_(ncomponents),
    U_(),
    s_(),
    V_()
{}

void
PCA::fit(const DynMat<real_t>& data){

    if(n_components_ > data.columns()){
        throw std::logic_error("Number of components cannot be larger than the number of features");
    }

    // all the features are requested
    if(n_components_ == 0){
        n_components_ = data.columns();
    }

    U_.clear();
    s_.clear();
    V_.clear();

    // do SVD
    DynMat<real_t> V;
    svd(data, U_, s_, V );

    if(n_components_ == data.columns()){
        V_ = std::move(V);
    }
    else{

        V_.resize(data.rows(), n_components_);

        for(uint_t c=0; c<n_components_; ++c){
            auto col = get_column(V, c);

            // set the column to V_;
        }

    }

    // get





}


}

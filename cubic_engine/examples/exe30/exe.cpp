#include "cubic_engine/base/cubic_engine_types.h"
#include "kernel/maths/matrix_utilities.h"
#include "kernel/utilities/data_set_loaders.h"
#include "kernel/maths/pca.h"
#include <cmath>
#include <iostream>
#include <tuple>

namespace example
{

using cengine::uint_t;
using cengine::real_t;
using cengine::DynMat;
using cengine::DynVec;

void test_case_1(){

    DynMat<real_t> X(6, 2);
    X(0,0) = -1.;
    X(0,1) = -1.;

    X(1,0) = -2.;
    X(1,1) = -1.;

    X(2,0) = -3.;
    X(2,1) = -2.;

    X(3,0) = 1.;
    X(3,1) = 1.;

    X(4,0) = 2.;
    X(4,1) = 1.;

    X(5,0) = 3.;
    X(5,1) = 2.;

    // caluclate the sample variance
    // of each of the 3 variables (columns)
    auto col1 = kernel::get_column(X, 0);
    auto col2 = kernel::get_column(X, 1);

    auto col1_var = var(col1);
    auto col2_var = var(col2);

    std::cout<<"Variable 1 variance: "<<col1_var<<std::endl;
    std::cout<<"Variable 2 variance: "<<col2_var<<std::endl;

    // compute the total variance
    auto total_var = col1_var + col2_var;

    std::cout<<"Total variance: "<<total_var<<std::endl;

    DynMat<real_t> U;
    DynVec<real_t> s;
    DynMat<real_t> V;


        std::cout<<"Variable 1 explains: "<<col1_var/total_var<<std::endl;
        std::cout<<"Variable 2 explains: "<<col2_var/total_var<<std::endl;

        svd(X, U, s, V );

        std::cout<<"Singular values: "<<s<<std::endl;

        // Principal axes in feature space,
        // representing the directions of maximum variance in the data.
        // these are the columns of the V matrix
        std::cout<<"V matrix: "<<V<<std::endl;

        // reconstruct the data set with PCA
        // The full principal components decomposition of
        // X can be given as T= XW
        DynMat<real_t> T = X*V;

        // caluclate the sample variance
        // of each of the 3 variables (columns)
        auto pca_col1 = kernel::get_column(T, 0);
        auto pca_col2 = kernel::get_column(T, 1);

        auto pca_col1_var = var(pca_col1);
        auto pca_col2_var = var(pca_col2);

        std::cout<<"PCA variable 1 variance: "<<pca_col1_var<<std::endl;
        std::cout<<"PCA variable 2 variance: "<<pca_col2_var<<std::endl;

        // this should be the same at the total variance
        // compute the total variance
        auto pca_total_var = pca_col1_var + pca_col2_var;

        std::cout<<"PCA Total variance: "<<pca_total_var<<std::endl;

        std::cout<<"PCA Variable 1 explains: "<<pca_col1_var/pca_total_var<<std::endl;
        std::cout<<"PCA Variable 2 explains: "<<pca_col2_var/pca_total_var<<std::endl;

}

void test_case_2(){

    using  kernel::PCA;

    // load the wine data set
    auto data = kernel::load_wine_data_set(false);

    // extract the column means
    auto means = kernel::get_column_means(data.first);

    // keep the first three components
    // with the largest variance
    PCA pca(3);

    pca.fit(data.first);



}

}

int main() {
   
    using namespace example;
    
    try{

        test_case_1();
    }
    catch(std::runtime_error& e){
        std::cerr<<"Runtime error: "
                 <<e.what()<<std::endl;
    }
    catch(std::logic_error& e){
        std::cerr<<"Logic error: "
                 <<e.what()<<std::endl;
    }
    catch(...){
        std::cerr<<"Unknown exception was raised whilst running simulation."<<std::endl;
    }
   
    return 0;
}


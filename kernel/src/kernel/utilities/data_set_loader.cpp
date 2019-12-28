#include "kernel/utilities/data_set_loaders.h"
namespace kernel
{

std::pair<DynMat<real_t>, DynVec<real_t>>
load_car_plant_dataset(uint_t label_idx, bool add_ones_column){

    uint_t ncols = add_ones_column?2:1;

    static real_t col1[] = {4.51, 3.58, 4.31,
                            5.06, 5.64, 4.99,
                            5.29, 5.83, 4.70,
                            5.61, 4.90, 4.20};

    static real_t col2[] = {2.48, 2.26, 2.47,
                            2.77, 2.99, 3.05,
                            3.18, 3.46, 3.03,
                            3.26, 2.67, 2.53};

    DynVec<real_t> labels;

    if(label_idx == 1 ){
        labels = col2;
    }
    else{
        labels = col1;
    }

    DynMat<real_t> mat(sizeof (col1)/sizeof (real_t), ncols, add_ones_column?1.0:0.0);

    if(add_ones_column){

        for(uint_t r=0; r<mat.rows(); ++r){
            mat(r, 1) = label_idx==1?col1[r]:col2[r];
        }
    }
    else{
        for(uint_t r=0; r<mat.rows(); ++r){
            mat(r, 0) = label_idx==1?col1[r]:col2[r];
        }
    }

    return std::pair(mat, labels);
}

}

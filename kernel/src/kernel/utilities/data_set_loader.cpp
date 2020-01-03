#include "kernel/utilities/data_set_loaders.h"
#include "kernel/parallel/utilities/array_partitioner.h"
#include "kernel/utilities/csv_file_reader.h"
#include "kernel/base/config.h"

#include <exception>
#include <cstdlib> //std::atof
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

std::pair<PartitionedType<DynMat<real_t>>,
PartitionedType<DynVec<real_t>>> load_car_plant_dataset_with_partitions(uint nparts, uint_t label_idx, bool add_ones_column){


    uint_t ncols = add_ones_column?2:1;

    static real_t col1[] = {4.51, 3.58, 4.31,
                            5.06, 5.64, 4.99,
                            5.29, 5.83, 4.70,
                            5.61, 4.90, 4.20};

    static real_t col2[] = {2.48, 2.26, 2.47,
                            2.77, 2.99, 3.05,
                            3.18, 3.46, 3.03,
                            3.26, 2.67, 2.53};

    PartitionedType<DynVec<real_t>> labels;

    if(label_idx == 1 ){
        labels = col2;
    }
    else{
        labels = col1;
    }

    PartitionedType<DynMat<real_t>> mat(sizeof (col1)/sizeof (real_t), ncols, add_ones_column?1.0:0.0);

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

    std::vector<range1d<uint_t>> partitions;
    partition_range(0, mat.rows(), partitions, nparts );
    mat.set_partitions(partitions);
    labels.set_partitions(partitions);

    return std::pair(std::move(mat), std::move(labels));

}


std::pair<DynMat<real_t>, DynVec<real_t>> load_car_plant_multi_dataset(uint_t labels_idx, bool add_ones_column){

    if(labels_idx != 0 || labels_idx != 1 || labels_idx != 2){

        throw std::invalid_argument("labels_idx should be 0 or 1 or 2 but got "+std::to_string(labels_idx));
    }

    uint_t ncols = add_ones_column?3:2;

    static real_t col0[] = {4.51, 3.58, 4.31,
                            5.06, 5.64, 4.99,
                            5.29, 5.83, 4.70,
                            5.61, 4.90, 4.20};

    static real_t col1[] = {0.0, 0.0, 13.0,
                            56.0, 117.0, 306.0,
                            358.0, 330.0, 187.0,
                            94.0, 23.0, 0.0};

    static real_t col2[] = {2.48, 2.26, 2.47,
                            2.77, 2.99, 3.05,
                            3.18, 3.46, 3.03,
                            3.26, 2.67, 2.53};

    DynVec<real_t> labels;

    if(labels_idx == 0 ){
        labels = col0;
    }
    else if(labels_idx == 1) {
        labels = col1;
    }
    else{
      labels = col2;
    }

    DynMat<real_t> mat(sizeof (col1)/sizeof (real_t), ncols, add_ones_column?1.0:0.0);

    if(add_ones_column){

         if(labels_idx == 0 ){

            for(uint_t r=0; r<mat.rows(); ++r){
                mat(r, 1) = col1[r];
                mat(r, 2) = col2[r];
            }
         }
         else if(labels_idx == 1 ){

            for(uint_t r=0; r<mat.rows(); ++r){
                mat(r, 1) = col0[r];
                mat(r, 2) = col2[r];
            }
         }
         else if(labels_idx == 2 ){

            for(uint_t r=0; r<mat.rows(); ++r){
                mat(r, 1) = col0[r];
                mat(r, 2) = col1[r];
            }
         }
    }
    else{
        if(labels_idx == 0 ){

           for(uint_t r=0; r<mat.rows(); ++r){
               mat(r, 0) = col1[r];
               mat(r, 1) = col2[r];
           }
        }
        else if(labels_idx == 1 ){

           for(uint_t r=0; r<mat.rows(); ++r){
               mat(r, 0) = col0[r];
               mat(r, 1) = col2[r];
           }
        }
        else if(labels_idx == 2 ){

           for(uint_t r=0; r<mat.rows(); ++r){
               mat(r, 0) = col0[r];
               mat(r, 1) = col1[r];
           }
        }
    }

    return std::pair(mat, labels);

}

std::pair<PartitionedType<DynMat<real_t>>,
          PartitionedType<DynVec<real_t>>> load_car_plant_multi_dataset_with_partitions(uint nparts,
                                                                                        uint_t labels_idx,
                                                                                        bool add_ones_column){

    if(labels_idx != 0 || labels_idx != 1 || labels_idx != 2){

        throw std::invalid_argument("labels_idx should be 0 or 1 or 2 but got "+std::to_string(labels_idx));
    }

    uint_t ncols = add_ones_column?3:2;

    static real_t col0[] = {4.51, 3.58, 4.31,
                            5.06, 5.64, 4.99,
                            5.29, 5.83, 4.70,
                            5.61, 4.90, 4.20};

    static real_t col1[] = {0.0, 0.0, 13.0,
                            56.0, 117.0, 306.0,
                            358.0, 330.0, 187.0,
                            94.0, 23.0, 0.0};

    static real_t col2[] = {2.48, 2.26, 2.47,
                            2.77, 2.99, 3.05,
                            3.18, 3.46, 3.03,
                            3.26, 2.67, 2.53};

    PartitionedType<DynVec<real_t>> labels;

    if(labels_idx == 0 ){
        labels = col0;
    }
    else if(labels_idx == 1) {
        labels = col1;
    }
    else{
      labels = col2;
    }

    PartitionedType<DynMat<real_t>> mat(sizeof (col1)/sizeof (real_t), ncols, add_ones_column?1.0:0.0);

    if(add_ones_column){

         if(labels_idx == 0 ){

            for(uint_t r=0; r<mat.rows(); ++r){
                mat(r, 1) = col1[r];
                mat(r, 2) = col2[r];
            }
         }
         else if(labels_idx == 1 ){

            for(uint_t r=0; r<mat.rows(); ++r){
                mat(r, 1) = col0[r];
                mat(r, 2) = col2[r];
            }
         }
         else if(labels_idx == 2 ){

            for(uint_t r=0; r<mat.rows(); ++r){
                mat(r, 1) = col0[r];
                mat(r, 2) = col1[r];
            }
         }
    }
    else{
        if(labels_idx == 0 ){

           for(uint_t r=0; r<mat.rows(); ++r){
               mat(r, 0) = col1[r];
               mat(r, 1) = col2[r];
           }
        }
        else if(labels_idx == 1 ){

           for(uint_t r=0; r<mat.rows(); ++r){
               mat(r, 0) = col0[r];
               mat(r, 1) = col2[r];
           }
        }
        else if(labels_idx == 2 ){

           for(uint_t r=0; r<mat.rows(); ++r){
               mat(r, 0) = col0[r];
               mat(r, 1) = col1[r];
           }
        }
    }

    std::vector<range1d<uint_t>> partitions;
    partition_range(0, mat.rows(), partitions, nparts );
    mat.set_partitions(partitions);
    labels.set_partitions(partitions);
    return std::pair(std::move(mat), std::move(labels));

}


std::pair<DynMat<real_t>, DynVec<uint_t>> load_reduced_iris_data_set(bool add_ones_column){

    CSVFileReader reader(DATA_SET_FOLDER);

    uint_t cols = add_ones_column?5:4;
    DynMat<real_t> matrix(100, cols, 0.0);
    DynVec<uint_t> labels(100);

    uint_t r = 0;
    while(!reader.eof()){

        auto line = reader.read_line();

        if(r != 0){

            std::vector<real_t> row(4, 0.0);

            for(uint_t i = 0; i<line.size()-1; ++i){
               row[i] = std::atof(line[i].c_str());
            }

            uint c = add_ones_column?1:0;
            for(; c<cols; ++c){
                matrix(r, c) = row[c];
            }

            if(line[4] == "Iris-setosa"){
                labels[r-1] = 0;
            }
            else if(line[4] == "Iris-versicolor"){
                labels[r-1] = 1;
            }
        }

        r++;
    }

    return std::make_pair(matrix, labels);

}

}

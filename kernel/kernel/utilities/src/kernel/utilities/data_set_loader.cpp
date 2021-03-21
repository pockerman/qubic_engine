#include "kernel/utilities/data_set_loaders.h"
#include "kernel/parallel/utilities/array_partitioner.h"
#include "kernel/utilities/csv_file_reader.h"
#include "kernel/base/kernel_consts.h"
#include "kernel/base/config.h"

#include <exception>
#include <cstdlib> //std::atof
#include <iostream>

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
PartitionedType<DynVec<real_t>>> 
load_car_plant_dataset_with_partitions(uint nparts, uint_t label_idx, bool add_ones_column){


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

    PartitionedType<DynMat<real_t>> mat(sizeof (col1)/sizeof (real_t), 
	                                     ncols, add_ones_column?1.0:0.0);

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


std::pair<DynMat<real_t>, DynVec<real_t>> 
load_car_plant_multi_dataset(uint_t labels_idx, bool add_ones_column){

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
          PartitionedType<DynVec<real_t>>> 
		  load_car_plant_multi_dataset_with_partitions(uint nparts,
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


std::pair<DynMat<real_t>, DynVec<uint_t>> 
load_reduced_iris_data_set(bool add_ones_column){

    std::string file(DATA_SET_FOLDER);
    file += "/iris_dataset_reduced.csv";

    CSVFileReader reader(file);

    uint_t cols = add_ones_column? 5 : 4;
    real_t val = add_ones_column ? 1.0 : 0.0;
    DynMat<real_t> matrix(100, cols, val);
    DynVec<uint_t> labels(100);

    // read the first line as this is the header
    reader.read_line();

    uint_t r = 0;

     while(!reader.eof()){

          auto line = reader.read_line();

          std::vector<real_t> row(4, 0.0);

          for(uint_t i = 0; i<line.size()-1; ++i){
             row[i] = std::atof(line[i].c_str());
          }

          uint c = add_ones_column?1:0;

          for(; c<cols; ++c){
              matrix(r, c) = row[ add_ones_column ? c-1 : c ];
          }

          if(line[4] == "Iris-setosa"){
              labels[r] = 0;
          }
          else if(line[4] == "Iris-versicolor"){
              labels[r] = 1;
          }
          else{
              throw std::invalid_argument("Unknown class in reduced iris dataset: "+line[4]);
          }

        r++;
      }

    return std::pair(matrix, labels);
}

std::pair<PartitionedType<DynMat<real_t>>,
          PartitionedType<DynVec<uint_t>>> 
		  load_reduced_iris_data_set_with_partitions(uint nparts,
													 bool add_ones_column){

    std::string file(DATA_SET_FOLDER);
    file += "/iris_dataset_reduced.csv";

    CSVFileReader reader(file);

    uint_t cols = add_ones_column? 5 : 4;
    real_t val = add_ones_column ? 1.0 : 0.0;
    PartitionedType<DynMat<real_t>> matrix(100, cols, val);
    PartitionedType<DynVec<uint_t>> labels(100);

    // read the first line as this is the header
    reader.read_line();

    uint_t r = 0;

     while(!reader.eof()){

          auto line = reader.read_line();

          std::vector<real_t> row(4, 0.0);

          for(uint_t i = 0; i<line.size()-1; ++i){
             row[i] = std::atof(line[i].c_str());
          }

          uint c = add_ones_column?1:0;

          for(; c<cols; ++c){
              matrix(r, c) = row[ add_ones_column ? c-1 : c ];
          }

          if(line[4] == "Iris-setosa"){
              labels[r] = 0;
          }
          else if(line[4] == "Iris-versicolor"){
              labels[r] = 1;
          }
          else{
              throw std::invalid_argument("Unknown class in reduced iris dataset: "+line[4]);
          }

        r++;
      }

     std::vector<range1d<uint_t>> partitions;
     partition_range(0, matrix.rows(), partitions, nparts );
     matrix.set_partitions(partitions);
     labels.set_partitions(partitions);
     return std::pair(std::move(matrix), std::move(labels));

}

/// \brief Load the iris data set and assigned
std::pair<DynMat<real_t>, DynVec<uint_t>> 
load_iris_data_set(bool add_ones_column){

    std::string file(DATA_SET_FOLDER);
    file += "/iris_data.csv";

    CSVFileReader reader(file);

    uint_t cols = add_ones_column? 5 : 4;
    real_t val = add_ones_column ? 1.0 : 0.0;
    DynMat<real_t> matrix(150, cols, val);
    DynVec<uint_t> labels(150);

    // read the first line as this is the header
    reader.read_line();

    uint_t r = 0;

     while(!reader.eof() && r < matrix.rows() ){

          auto line = reader.read_line();

          std::vector<real_t> row(4, 0.0);

          for(uint_t i = 0; i<line.size()-1; ++i){
             row[i] = std::atof(line[i].c_str());
          }

          uint c = add_ones_column?1:0;

          for(; c<cols; ++c){
              matrix(r, c) = row[ add_ones_column ? c-1 : c ];
          }

          if(line[4] == "Iris-setosa"){
              labels[r] = 0;
          }
          else if(line[4] == "Iris-versicolor"){
              labels[r] = 1;
          }
          else if(line[4] == "Iris-virginica"){
              labels[r] = 2;
          }
          else{
              throw std::invalid_argument("Unknown class in reduced iris dataset: "+line[4]);
          }

        r++;
      }

     return std::pair(std::move(matrix), std::move(labels));
}

std::pair<PartitionedType<DynMat<real_t>>,
          PartitionedType<DynVec<uint_t>>> 
		  load_iris_data_set_with_partitions(uint nparts,
											 bool add_ones_column){

    std::string file(DATA_SET_FOLDER);
    file += "/iris_data.csv";

    CSVFileReader reader(file);

    uint_t cols = add_ones_column? 5 : 4;
    real_t val = add_ones_column ? 1.0 : 0.0;
    PartitionedType<DynMat<real_t>> matrix(150, cols, val);
    PartitionedType<DynVec<uint_t>> labels(150);

    // read the first line as this is the header
    reader.read_line();

    uint_t r = 0;

     while(!reader.eof() && r < matrix.rows() ){

          auto line = reader.read_line();

          std::vector<real_t> row(4, 0.0);

          for(uint_t i = 0; i<line.size()-1; ++i){
             row[i] = std::atof(line[i].c_str());
          }

          uint c = add_ones_column?1:0;

          for(; c<cols; ++c){
              matrix(r, c) = row[ add_ones_column ? c-1 : c ];
          }

          if(line[4] == "Iris-setosa"){
              labels[r] = 0;
          }
          else if(line[4] == "Iris-versicolor"){
              labels[r] = 1;
          }
          else if(line[4] == "Iris-virginica"){
              labels[r] = 2;
          }
          else{
              throw std::invalid_argument("Unknown class in reduced iris dataset: "+line[4]);
          }

        r++;
      }

     std::vector<range1d<uint_t>> partitions;
     partition_range(0, matrix.rows(), partitions, nparts );
     matrix.set_partitions(partitions);
     labels.set_partitions(partitions);
     return std::pair(std::move(matrix), std::move(labels));

}


std::pair<DynMat<real_t>, DynVec<real_t>>
load_x_y_sinuisoid_data_set(bool add_ones_column){

    std::string file(DATA_SET_FOLDER);
    file += "/X_Y_Sinusoid_Data.csv";

    CSVFileReader reader(file);

    uint_t cols = add_ones_column? 2 : 1;
    real_t val = add_ones_column ? 1.0 : 0.0;
    DynMat<real_t> matrix(20, cols, val);
    DynVec<real_t> labels(20);

    // read the first line as this is the header
    reader.read_line();

    uint_t r = 0;

     while(!reader.eof()){

         if(r==20)
             break;

          auto line = reader.read_line();

          std::vector<real_t> row(line.size(), 0.0);

          for(uint_t i = 0; i<line.size(); ++i){
             row[i] = std::atof(line[i].c_str());
          }

          if(add_ones_column){
              matrix(r, 1) = row[0];
          }
          else{
             matrix(r, 0) = row[0];
          }

          labels[r] = row[1];
          r++;
      }

      return std::pair(std::move(matrix), std::move(labels));
}

/// \brief Load the reduced iris data set and assigned partitions
std::pair<PartitionedType<DynMat<real_t>>,
          PartitionedType<DynVec<real_t>>> 
		  load_x_y_sinuisoid_data_set_with_partitions(uint nparts,
													  bool add_ones_column){
    std::string file(DATA_SET_FOLDER);
    file += "/X_Y_Sinusoid_Data.csv";

    CSVFileReader reader(file);

    uint_t cols = add_ones_column? 2 : 1;
    real_t val = add_ones_column ? 1.0 : 0.0;
    PartitionedType<DynMat<real_t>> matrix(20, cols, val);
    PartitionedType<DynVec<real_t>> labels(20);

    // read the first line as this is the header
    reader.read_line();

    uint_t r = 0;

     while(!reader.eof()){

         if(r==20)
             break;

          auto line = reader.read_line();

          std::vector<real_t> row(line.size(), 0.0);

          for(uint_t i = 0; i<line.size(); ++i){
             row[i] = std::atof(line[i].c_str());
          }

          if(add_ones_column){
              matrix(r, 1) = row[0];
          }
          else{
             matrix(r, 0) = row[0];
          }

          labels[r] = row[1];
          r++;
      }

     std::vector<range1d<uint_t>> partitions;
     partition_range(0, matrix.rows(), partitions, nparts );
     matrix.set_partitions(partitions);
     labels.set_partitions(partitions);
     return std::pair(std::move(matrix), std::move(labels));

}

DynMat<real_t> load_kmeans_test_data(){

    DynMat<real_t> data(6, 2);

    data(0, 0) = 1.0;
    data(0, 1) = 2.0;
    data(1, 0) = 1.0;
    data(1, 1) = 4.0;
    data(2, 0) = 1.0;
    data(2, 1) = 0.0;
    data(3, 0) = 10.0;
    data(3, 1) = 2.0;
    data(4, 0) = 10.0;
    data(4, 1) = 4.0;
    data(5, 0) = 10.0;
    data(5, 1) = 0.0;

    return data;

}

std::pair<DynMat<real_t>,
          DynVec<uint_t>> load_wine_data_set(bool add_ones_column){

    uint_t ncols = add_ones_column?13:12;
    uint col_start = add_ones_column?1:0;
    uint_t nrows = 178;

    std::string file(DATA_SET_FOLDER);
    file += "/wine.data";

    CSVFileReader reader(file);
    DynMat<real_t> matrix(nrows, ncols, 0.0);

    if(add_ones_column){
        for(uint_t r=0; r<matrix.rows(); ++r){
            matrix(r, 0) = 1.0;
        }
    }

    // the labels
    DynVec<uint_t> labels(nrows, 0);

    for (uint_t r=0; r<nrows; ++r){

        auto line = reader.read_line();

        labels[r] = std::stoul (line[0], nullptr, 10);

        std::vector<real_t> vals(line.size()-1, 0.0);

        for(uint_t c=0; c<vals.size(); ++c){
           vals[c] = std::atof(line[c+1].c_str());
        }

        uint_t clocal = 0;
        for(uint c=col_start; c<matrix.columns(); ++c){
           matrix(r, c) =  vals[clocal++];
        }

    }


    return {matrix, labels};
}


void 
load_random_set_one(DynMat<real_t>& matrix){
	
	std::string file(DATA_SET_FOLDER);
	file += "/random_data_1.txt";
	
	// resize the matrix 
	matrix.resize(100, 2);
	
	CSVFileReader reader(file, true, " ");
	
	auto row_counter = 0;
	while(!reader.eof()){
		
		auto line = reader.read_line();
		
		if(line[0] == KernelConsts::eof_string()){
			break;
		}
		
		
		if(line[0] == "#"){
			continue;
		}
		
		if(line.size() != 2){
			break; //EOF
		}
		
		std::vector<real_t> vals(line.size(), 0.0);

        for(uint_t c=0; c<vals.size(); ++c){
           vals[c] = std::atof(line[c].c_str());
        }
		
		
        for(uint c=0; c<matrix.columns(); ++c){
           matrix(row_counter, c) =  vals[c];
        }
		
		row_counter++;
	}
}

}

#ifndef MATRIX_UTILITIES_H
#define MATRIX_UTILITIES_H

#include "kernel/base/types.h"

#include <random>
#include <set>

namespace kernel
{

/**
 * @brief Exchange the rows of the matrix
 */
template<typename MatType>
void exchange_rows(MatType& mat, uint_t r1, uint_t r2){

    for(uint_t col=0; col<mat.columns(); ++col){

        auto tmp = mat(r1, col);
        mat(r1, col) = mat(r2, col);
        mat(r2, col) = tmp;
    }
}

/**
 * @brief Shuffle the rows of the matrix. Returns a set with the rows that have been shuffled
 */
template<typename MatType>
std::set<uint_t> shuffle_matrix_rows(MatType& mat){

    uint_t min = 0;
    uint_t max = mat.rows();

    std::random_device rd;  //Will be used to obtain a seed for the random number engine
    std::mt19937 gen(rd());
    std::uniform_int_distribution<uint_t> random(min, max);

    std::set<uint_t> touched;

    for(uint_t r=0; r<max; ++r){

        // this row was touched
        if(touched.find(r) != touched.end()){
            continue;
        }

        uint_t candidate = random(gen);

        if(touched.find(candidate) != touched.end()){
            continue;
        }

        exchange_rows(mat, r, candidate);
        touched.insert(r);
        touched.insert(candidate);
    }

    return touched;
}

template<typename T>
DynVec<T>
get_row(const DynMat<T>& matrix, uint_t row_idx){

    DynVec<T> row(matrix.columns());

    auto it = matrix.begin(row_idx);
    auto it_end = matrix.end(row_idx);

    uint_t col_counter = 0;
    for(; it != it_end; ++it){
        auto val = *it;
        row[col_counter++] =  val;
    }

    return row;
}


template<typename T>
void extract_randomly(const DynMat<T>& matrix, std::vector<DynVec<T>>& rsult,
                      uint_t how_many, bool allow_duplicates=false){

    rsult.clear();
    rsult.reserve(how_many);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, matrix.rows() - 1);
    std::set<uint_t> touched;

    while(rsult.size() != how_many){

        uint_t idx = dis(gen);

        // this row was touched
        if((touched.find(idx) != touched.end()) && !allow_duplicates){
            continue;
        }

        rsult.push_back(get_row(matrix, idx));
        touched.insert(idx);
    }
}
}

#endif // MATRIX_SHUFFLER_H

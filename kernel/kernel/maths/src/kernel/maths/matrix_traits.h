#ifndef MATRIX_TRAITS_H
#define MATRIX_TRAITS_H

#include "kernel/base/types.h"
#include "kernel/parallel/utilities/partitioned_type.h"

namespace kernel
{

template<typename MatrixType>
struct matrix_row_trait;

template<typename T>
struct matrix_row_trait<DynMat<T>>
{

    typedef T value_t;
    typedef DynVec<T> row_t;

    ///
    /// \brief get_row Returns the row with index row_idx
    ///
    static row_t get_row(const DynMat<T>& matrix, uint_t row_idx);
};

template<typename T>
typename matrix_row_trait<DynMat<T>>::row_t
matrix_row_trait<DynMat<T>>::get_row(const DynMat<T>& matrix, uint_t row_idx){

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
struct matrix_row_trait<PartitionedType<DynMat<T>>>
{

    typedef T value_t;
    typedef DynVec<T> row_t;

    ///
    /// \brief get_row Returns the row with index row_idx
    ///
    static row_t get_row(const PartitionedType<DynMat<T>>& matrix, uint_t row_idx);
};


template<typename T>
typename matrix_row_trait<PartitionedType<DynMat<T>>>::row_t
matrix_row_trait<PartitionedType<DynMat<T>>>::get_row(const PartitionedType<DynMat<T>>& matrix, uint_t row_idx){

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

}

#endif // MATRIX_TRAITS_H

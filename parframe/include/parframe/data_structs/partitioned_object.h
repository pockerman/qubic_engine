#ifndef ROW_PARTITIONED_MATRIX_H
#define ROW_PARTITIONED_MATRIX_H

#include "parframe/data_structs/range_1d.h"
#include <vector>

namespace parframe
{

 /**
  * A utility data structure that provides access to
  * partitioned indexes to the given  type
  **/

template<typename Type>
class PartitionedType: public Type
{

public:

    /// \brief The type of the object that is partitioned
    typedef Type object_type;

    /// \brief Allow using the constructors of Type
    using Type::Type;

    /// \brief how many partitions the patrix has
    uint_t n_partitions()const{return partitions_.size();}

    /// \brief Returns true if this object has partitions assigned to it
    bool has_partitions()const{return !partitions_.empty();}

    /// \brief Set the partitions
    void set_partitions(const std::vector<range1d<uint_t>>& parts){partitions_ = parts;}

    /// \brief Returns the partitions
    const auto& get_partitions()const{return partitions_;}

    /// \brief Return the t-th partiton. Throws std::invalid_argument if t
    /// is out of  bounds
    const range1d<uint_t> get_partition(uint_t t)const;

private:

    /// \brief Hold the row indices or each partition
    std::vector<range1d<uint_t>> partitions_;

};

template<typename Type>
const range1d<uint_t>
PartitionedType<Type>::get_partition(uint_t t)const{

    if(t >= partitions_.size()){
        throw std::invalid_argument(std::to_string(t)+" not in [0, "+std::to_string(partitions_.size())+")");
    }

    return partitions_[t];
}



}

#endif // ROW_PARTITIONED_MATRIX_H

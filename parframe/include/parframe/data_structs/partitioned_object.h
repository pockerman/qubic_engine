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

    typedef Type object_type;

    /// \brief how many partitions the patrix has
    uint_t n_partitions()const{return row_partitions_.size();}

    /// \brief Set the partitions
    void set_partitions(std::vector<range1d<uint_t>>&& parts){partitions_ = std::move(parts);}

private:

    /// \brief Hold the row indices or each partition
    std::vector<range1d<uint_t>> partitions_;

};
}

#endif // ROW_PARTITIONED_MATRIX_H

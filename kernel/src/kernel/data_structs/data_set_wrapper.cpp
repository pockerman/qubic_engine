#include "kernel/data_structs/data_set_wrapper.hpp"
#include "kernel/maths/matrix_traits.h"

#include <stdexcept>

namespace kernel
{
	namespace data_structs
	{
		
		template<typename T>
		DataSetWrapper<DynMat<T>>::DataSetWrapper()
		:
		data_()
		{}
		
		template<typename T>
		DataSetWrapper<DynMat<T>>::~DataSetWrapper()
		{}
		
		template<typename T>
		typename DataSetWrapper<DynMat<T>>::row_t 
		DataSetWrapper<DynMat<T>>::get_row(uint_t row_idx)const{
			
			if(row_idx >= data_.rows()){
				throw std::logic_error("Invalid row index Index="+
				std::to_string(row_idx) + 
				" not in [0," +
				std::to_string(data_.rows()) +
				")");
			}
			
			return matrix_row_trait<DynMat<T>>::get_row(data_, row_idx);
		}
		
		
		// explicit instantiation
		template class DataSetWrapper<DynMat<real_t>>;
		template class DataSetWrapper<DynMat<uint_t>>;
		template class DataSetWrapper<DynMat<int>>;
	}
}
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
		DataSetWrapper<DynMat<T>>::DataSetWrapper(uint_t nrows, uint_t ncols)
		:
		data_(nrows, ncols)
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
		
		template<typename T>
		void 
		DataSetWrapper<DynMat<T>>::set_row(uint_t r, const typename DataSetWrapper<DynMat<T>>::row_t& row){
			
			if(data_.rows() != 0 && data_.columns() != row.size()){
				
				throw std::logic_error("Invalid row schema. Row size should be"+
				std::to_string(data_.columns()) + 
				" but is" +
				std::to_string(row.size()) +
				")");
			}
			
			for(uint_t c=0; c<data_.columns(); ++c){
				data_(r, c) = row[c];
			}
			
		}
		
		// explicit instantiation
		template class DataSetWrapper<DynMat<real_t>>;
		template class DataSetWrapper<DynMat<uint_t>>;
		template class DataSetWrapper<DynMat<int>>;
	}
}
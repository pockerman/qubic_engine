#ifndef SOFTMAX_FUNCTION_H
#define SOFTMAX_FUNCTION_H

#include "kernel/maths/functions/function_base.h"
#include <cmath>

namespace kernel
{
	namespace maths
	{
		namespace functions
		{
			template<typename VectorTp>
			class SoftMaxFunction: public FunctionBase<VectorTp, VectorTp>
			{
			public:
				
				typedef VectorTp input_t;
				typedef VectorTp output_t;
				
			};
		}
	}
}

#endif 
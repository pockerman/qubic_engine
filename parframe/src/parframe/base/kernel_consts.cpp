#include "parframe/base/kernel_consts.h"

namespace kernel
{
    real_t
	kernel_consts::tol_ = 1.0e-8;

	void 
	kernel_consts::initialize(){

		kernel_consts::tol_ = 1.0e-8;
	}

	kernel_consts::kernel_consts()
	{}
}

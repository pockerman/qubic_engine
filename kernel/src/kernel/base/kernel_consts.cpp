#include "kernel/base/kernel_consts.h"

namespace kernel
{
    real_t
	KernelConsts::tol_ = 1.0e-8;

	void 
	KernelConsts::initialize(){

		KernelConsts::tol_ = 1.0e-8;
	}

}

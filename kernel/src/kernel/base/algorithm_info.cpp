#include "kernel/base/algorithm_info.h"

namespace kernel
{
    
std::ostream& 
AlgInfo::print(std::ostream& out)const{
    
    out<<"# iterations: "<<niterations<<std::endl;
    out<<"# processors: "<<nprocs<<std::endl;
    out<<"# threads: "<<nthreads<<std::endl;
    out<<"Residual: "<<residual<<std::endl;
    out<<"Convergence: "<<(converged?"Yes":"No")<<std::endl;
    out<<"Total time: "<<runtime.count()<<std::endl;
    
    return out;
}  
    
}

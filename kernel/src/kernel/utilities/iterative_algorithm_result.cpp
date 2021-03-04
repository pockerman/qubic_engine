#include "kernel/utilities/iterative_algorithm_result.h"

namespace kernel{

std::ostream&
IterativeAlgorithmResult:: print(std::ostream& out)const{

    out<<"Converged...: "<<std::boolalpha<<converged<<std::endl;
    out<<"Tolerance...: "<<tolerance<<std::endl;
    out<<"Residual....: "<<residual<<std::endl;
    out<<"Iterations..: "<<num_iterations<<std::endl;
    out<<"Total time..: "<<total_time.count()<<std::endl;
    return out;
}
}

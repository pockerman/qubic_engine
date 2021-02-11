
#ifdef _OPENMP

#include "kernel/utilities/algorithm_info.h"
#include "kernel/utilities/iterative_algorithm_controller.h"
#include "kernel/base/types.h"
#include "kernel/base/kernel_consts.h"


#include <blaze/Math.h>

#include <thread>
#include <vector>
#include <iostream>
#include <stdexcept>


namespace  {

    using kernel::uint_t;
    using kernel::real_t;
    using kernel::IterativeAlgorithmController;
    using kernel::AlgInfo;

    using Vector = blaze::DynamicVector<real_t>;
    using Matrix = blaze::DynamicMatrix<real_t>;
	
	

class JacobiIterator
{

public:

    JacobiIterator(const IterativeAlgorithmController& control )
        :
      ctrl_(control)
    {}

	
    AlgInfo iterate(const Matrix& mat, const Vector& b, Vector& x);

private:

    
    IterativeAlgorithmController ctrl_;

};


AlgInfo
JacobiIterator::iterate(const Matrix& mat, const Vector& b, Vector& x){
	
	

    AlgInfo info;
    info.nthreads = ctrl_.get_num_threads();

	// the old solution
    Vector old_solution(x);
	
	while(ctrl_.continue_iterations()){
		
		#pragma omp parallel shared(x, b, old_solution, mat)
		{
			#pragma omp for
			for(uint_t i=0; i<x.size(); ++i){
				
				real_t sum = 0.0;
				
				for(uint_t col=0; col < old_solution.size(); ++col){

					if( col != i){
						sum += mat(i, col)*old_solution[col];
					}
				}
				
				real_t a_ii = mat(i, i);
				real_t b_i  = b[i];
				x[i] = (1./a_ii)*(b_i - sum);
			}
		}
		
		/// compute the difference between the old and the computed solution
        real_t res = l2Norm(x - old_solution);
		
		ctrl_.update_residual(res);
		
		info.residual = res;
        info.niterations = ctrl_.get_current_iteration();

        if( res < ctrl_.get_exit_tolerance()){
            info.converged = true;
            break;
        }

        // set the old solution to the current one
        old_solution = x;
	}

    return info;
}

}

int main(){



	// Explicitly disable dynamic teams
	omp_set_dynamic(0); 

	// Use 4 threads for all consecutive parallel regions
    omp_set_num_threads(2);

#pragma omp parallel
{
    #pragma omp single
    {
            // Task #1
            #pragma  omp task
            {
                std::cout<<"Race "<<std::endl;
            }

            // Task #2
            #pragma  omp task
            {
                std::cout<<"car "<<std::endl;
            }

    }
}
    


    return 0;
}
#else

#include <iostream>

int main(){
	std::cout<<"This example requires OPENMP configuration\n";
    return 0;
}

#endif

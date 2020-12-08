
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

    using kernel::IterativeAlgorithmController;
    using kernel::AlgInfo;

    Vector b(100, 2.0);
    Vector x(100, 0.0);
    Matrix A(100, 100, 0.0);

    /// diagonalize the matrix
    for(uint_t r=0; r < A.rows(); ++r){
        for(uint_t c=0; c < A.columns(); ++c){

            if(r == c){
                A(r,c) = 1.0;
            }
        }
    }

	// Explicitly disable dynamic teams
	omp_set_dynamic(0); 

	// Use 4 threads for all consecutive parallel regions
	omp_set_num_threads(4); 
    
    // create the control
    IterativeAlgorithmController control(100, kernel::KernelConsts::tolerance());
	control.set_num_threads(4);

    // the object responsible for Jacobi iteration
    JacobiIterator iterator(control);

    // this will block until all iterations finish
    auto info = iterator.iterate(A, b, x);

    // print useful information
    std::cout<<info<<std::endl;

    // uncomment this to view the solution
    std::cout<<x<<std::endl;

    return 0;
}
#else

#include <iostream>

int main(){
	std::cout<<"This example requires OPENMP configuration\n";
    return 0;
}

#endif

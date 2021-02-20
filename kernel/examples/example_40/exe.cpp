
#ifdef _OPENMP

#include <omp.h>
#include <thread>
#include <vector>
#include <iostream>
#include <stdexcept>


namespace  {

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

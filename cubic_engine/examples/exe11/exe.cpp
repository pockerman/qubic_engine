//#include "kernel/base/config.h"
#include "cubic_engine/base/config.h"

#ifdef USE_PYTORCH
#include "torch/torch.h"

#include <iostream>

       
int main(){

    torch::Tensor tensor = torch::eye(3);
    std::cout << tensor << std::endl;
    return 0;
}
#else
#include <iostream>
int main(){
    std::cout<<"This example requires PyTorch configuration"<<std::endl;
    return 0;
}
#endif


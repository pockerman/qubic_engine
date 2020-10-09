#include "cubic_engine/base/cubic_engine_types.h"
#include "cubic_engine/ml/supervised_learning/multinomial_naive_bayes_classifier.h"
#include "kernel/utilities/data_set_loaders.h"
#include "kernel/maths/matrix_traits.h"

#include <iostream>

namespace example
{

using cengine::uint_t;
using cengine::real_t;
using cengine::DynMat;
using cengine::DynVec;
using cengine::ml::MultinomialNBC;

}

int main() {
   
    using namespace example;
    
    try{

        // load data
        auto data = kernel::load_reduced_iris_data_set(false);

        MultinomialNBC<decltype (data.first),
                       decltype(data.second)> mnbc;

        mnbc.train(data.first, data.second);

        auto p1 = kernel::matrix_row_trait<decltype (data.first)>::get_row(data.first, 0);

        auto pred = mnbc.predict(p1);

        std::cout<<"Class predicted: "<<pred.first<<" log probability: "<<pred.second<<std::endl;

    }
    catch(std::runtime_error& e){
        std::cerr<<"Runtime error: "
                 <<e.what()<<std::endl;
    }
    catch(std::logic_error& e){
        std::cerr<<"Logic error: "
                 <<e.what()<<std::endl;
    }
    catch(...){
        std::cerr<<"Unknown exception was raised whilst running simulation."<<std::endl;
    }
   
    return 0;
}


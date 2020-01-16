
#include "cubic_engine/base/cubic_engine_types.h"
#include "cubic_engine/ml/supervised_learning/logistic_regression.h"
#include "cubic_engine/optimization/batch_gradient_descent_wrapper.h"
#include "cubic_engine/optimization/utils/gd_control.h"

#include "kernel/maths/functions/real_vector_polynomial.h"
#include "kernel/maths/errorfunctions/mse_function.h"
#include "kernel/maths/functions/sigmoid_function.h"
#include "kernel/utilities/data_set_loaders.h"
#include "kernel/parallel/threading/thread_pool.h"

#include <iostream>

int main(){

    using cengine::uint_t;
    using cengine::real_t;
    using cengine::DynMat;
    using cengine::DynVec;
    using cengine::GDControl;
    using cengine::BatchGradientDescentWrapper;
    using cengine::LogisticRegression;
    using cengine::Null;
    using kernel::RealVectorPolynomialFunction;
    using kernel::MSEFunction;
    using kernel::SigmoidFunction;
    using kernel::ThreadPool;
    using kernel::PartitionedType;

    typedef RealVectorPolynomialFunction hypothesis_t;
    typedef SigmoidFunction<RealVectorPolynomialFunction> transformer_t;



    try{

        // the classifier to use. use a hypothesis of the form
        // f = w_0 + w_1*x_1 + w_2*x_2 + w_3*x_3 + w_4*x_4;
        // set initial weights to 0
        LogisticRegression<hypothesis_t, transformer_t> classifier({0.0, 0.0, 0.0, 0.0, 0.0});
        transformer_t sigmoid_h(classifier.get_model());

        {

            std::cout<<"Serial GD..."<<std::endl;

            typedef MSEFunction<transformer_t, DynMat<real_t>, DynVec<uint_t>> error_t;
            GDControl<error_t> control(10000, kernel::KernelConsts::tolerance(), GDControl<error_t>::DEFAULT_LEARNING_RATE);
            control.show_iterations = false;
            control.err_function.set_hypothesis_function(sigmoid_h);

            /// load the dataset
            auto dataset = kernel::load_reduced_iris_data_set();

            Null executor;
            // this is a serial implmentation
            BatchGradientDescentWrapper<error_t, Null, Null> gd(control, executor, Null() );

            auto result = classifier.train(dataset.first, dataset.second, gd);
            std::cout<<result<<std::endl;

            DynVec<real_t> point{1.0, 5.7, 2.8, 4.1, 1.3};
            auto class_idx = classifier.predict(point);

            std::cout<<"Class index: "<<class_idx<<std::endl;
        }

        {
            std::cout<<"Threaded GD..."<<std::endl;

            /// a thread pool with 4 threads
            ThreadPool executor(4);

            /// reset the model parameters
            classifier.set_model_parameters({0.0, 0.0, 0.0, 0.0, 0.0});

            typedef MSEFunction<transformer_t,
                        PartitionedType<DynMat<real_t>>,
                        PartitionedType<DynVec<uint_t>>> error_t;

            /// we need a partitioned data set
            auto dataset = kernel::load_reduced_iris_data_set_with_partitions(executor.get_n_threads());

            GDControl<error_t> control(10000, kernel::KernelConsts::tolerance(), GDControl<error_t>::DEFAULT_LEARNING_RATE);
            control.show_iterations = false;
            control.err_function.set_hypothesis_function(sigmoid_h);

            // this is a serial implmentation
            BatchGradientDescentWrapper<error_t, ThreadPool, Null> gd(control, executor, Null() );

            auto result = classifier.train(dataset.first, dataset.second, gd);
            std::cout<<result<<std::endl;

            DynVec<real_t> point{1.0, 5.7, 2.8, 4.1, 1.3};
            auto class_idx = classifier.predict(point);

            std::cout<<"Class index: "<<class_idx<<std::endl;

        }
    }
    catch(std::exception& e){

        std::cerr<<e.what()<<std::endl;
    }
    catch(...){

        std::cerr<<"Unknown exception occured"<<std::endl;
    }

    return 0;
}


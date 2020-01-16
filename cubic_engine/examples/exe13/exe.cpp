
#include "cubic_engine/base/cubic_engine_types.h"
#include "cubic_engine/ml/supervised_learning/logistic_regression.h"
#include "cubic_engine/optimization/serial_batch_gradient_descent.h"
#include "cubic_engine/optimization/utils/gd_control.h"
#include "cubic_engine/maths/confusion_matrix.h"

#include "kernel/maths/functions/real_vector_polynomial.h"
#include "kernel/maths/errorfunctions/mse_function.h"
#include "kernel/maths/functions/sigmoid_function.h"
#include "kernel/utilities/data_set_loaders.h"

#include <iostream>

int main(){

    using cengine::uint_t;
    using cengine::real_t;
    using cengine::DynMat;
    using cengine::DynVec;
    using cengine::GDControl;
    using cengine::Gd;
    using cengine::LogisticRegression;
    using cengine::ConfusionMatrix;
    using kernel::RealVectorPolynomialFunction;
    using kernel::MSEFunction;
    using kernel::SigmoidFunction;

    typedef RealVectorPolynomialFunction hypothesis_t;
    typedef SigmoidFunction<RealVectorPolynomialFunction> transformer_t;
    typedef MSEFunction<transformer_t, DynMat<real_t>, DynVec<uint_t>> error_t;

    try{

        auto dataset = kernel::load_reduced_iris_data_set();

        // the classifier to use. use a hypothesis of the form
        // f = w_0 + w_1*x_1 + w_2*x_2 + w_3*x_3 + w_4*x_4;
        // set initial weights to 0
        LogisticRegression<hypothesis_t, transformer_t> classifier({0.0, 0.0, 0.0, 0.0, 0.0});

        transformer_t sigmoid_h(classifier.get_model());

        GDControl<error_t> control(20000, 1.0e-4, 0.005);
        control.show_iterations = true;
        control.err_function.set_hypothesis_function(sigmoid_h);
        Gd<error_t> gd(control);

        auto result = classifier.train(dataset.first, dataset.second, gd);
        std::cout<<result<<std::endl;
        std::cout<<classifier<<std::endl;

        DynVec<real_t> point{1.0, 5.7, 2.8, 4.1, 1.3};
        auto class_idx = classifier.predict(point);

        std::cout<<"Class index: "<<class_idx<<std::endl;

        // predictions on the training set
        DynVec<uint_t> predictions(dataset.first.rows(), 0);
        classifier.predict(dataset.first, predictions);

        ConfusionMatrix cmatrix(dataset.second, predictions, 2);

        // let's do some calculations
        std::cout<<"\nConsfusion Matrix...."<<std::endl;
        std::cout<<cmatrix<<std::endl;

        std::cout<<"Accuracy.........."<<cmatrix.accuracy()<<std::endl;
        std::cout<<"True positives...."<<cmatrix.true_positives()<<std::endl;
        std::cout<<"Recall class 0...."<<cmatrix.recall_class(0)<<std::endl;
        std::cout<<"Recall class 1...."<<cmatrix.recall_class(1)<<std::endl;

    }
    catch(std::exception& e){

        std::cerr<<e.what()<<std::endl;
    }
    catch(...){

        std::cerr<<"Unknown exception occured"<<std::endl;
    }

    return 0;
}


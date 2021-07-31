#include "cubic_engine/base/cubic_engine_types.h"
#include "cubic_engine/ml/supervised_learning/logistic_regression.h"
#include "cubic_engine/maths/confusion_matrix.h"
#include "cubic_engine/ml/datasets/blaze_regression_dataset.h"
#include "cubic_engine/ml/datasets/data_set_loaders.h"

#include "kernel/numerics/optimization/gd_control.h"
#include "kernel/numerics/optimization/serial_gradient_descent.h"

#include <map>
#include <string>
#include <any>
#include <iostream>

int main(){

    using cengine::uint_t;
    using cengine::real_t;
    using cengine::DynMat;
    using cengine::DynVec;
    using cengine::ml::BlazeRegressionDataset;
    using cengine::ml::LogisticRegression;
    using kernel::numerics::opt::Gd;
    using kernel::numerics::opt::GDConfig;

    using cengine::uint_t;
    using cengine::real_t;
    using cengine::DynMat;
    using cengine::DynVec;
    using kernel::numerics::opt::Gd;
    using kernel::numerics::opt::GDConfig;
    using cengine::ml::LogisticRegression;
    using cengine::ConfusionMatrix;



    try{

        auto data = cengine::ml::load_reduced_iris_data_set();

        BlazeRegressionDataset dataset;
        dataset.load_from_data(data.first, data.second);

        // the classifier to use. use a hypothesis of the form
        // f = w_0 + w_1*x_1 + w_2*x_2 + w_3*x_3 + w_4*x_4;
        // set initial weights to 0
        LogisticRegression classifier(4,  true);

        GDConfig gd_config(10000, 1.0e-8, 0.01);
        Gd<DynMat<real_t>, DynVec<real_t>> solver(gd_config);

        auto result = classifier.fit(dataset, solver, std::map<std::string, std::any>());
        std::cout<<result<<std::endl;
        std::cout<<classifier<<std::endl;

        /*DynVec<real_t> point{1.0, 5.7, 2.8, 4.1, 1.3};
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
        std::cout<<"Recall class 1...."<<cmatrix.recall_class(1)<<std::endl;*/

    }
    catch(std::exception& e){

        std::cerr<<e.what()<<std::endl;
    }
    catch(...){

        std::cerr<<"Unknown exception occured"<<std::endl;
    }

    return 0;
}


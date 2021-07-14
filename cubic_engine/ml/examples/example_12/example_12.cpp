#include "cubic_engine/base/cubic_engine_types.h"
#include "cubic_engine/ml/supervised_learning/linear_regressor.h"
#include "cubic_engine/ml/supervised_learning/regularizer_type.h"
#include "cubic_engine/ml/datasets/blaze_regression_dataset.h"
#include "cubic_engine/ml/datasets/data_set_loaders.h"


#include <iostream>
#include <map>
#include <string>
#include <any>

int main(){

    using cengine::uint_t;
    using cengine::real_t;
    using cengine::DynMat;
    using cengine::DynVec;
    using cengine::ml::BlazeRegressionDataset;
    using cengine::ml::LinearRegressor;

    try{

        // load the data set
        BlazeRegressionDataset dataset;
        //auto dataset = kernel::load_car_plant_dataset();

        // linear model trained with OLS
        LinearRegressor regressor(1, true, cengine::ml::RegularizerType::NONE);

        // train the model
        std::map<std::string, std::any> options;
        regressor.fit(dataset, options);

        std::cout<<"Intercept: "<<regressor.get_interception()<<" slope: "<<regressor.get_parameters()[1]<<std::endl;


    }
    catch(std::exception& e){

        std::cerr<<e.what()<<std::endl;
    }
    catch(...){

        std::cerr<<"Unknown exception occured"<<std::endl;
    }

    return 0;
}


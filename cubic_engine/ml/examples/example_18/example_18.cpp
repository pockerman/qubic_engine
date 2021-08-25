#include "cubic_engine/base/cubic_engine_types.h"
#include "cubic_engine/ml/instance_learning/knn_classifier.h"
#include "cubic_engine/ml/instance_learning/knn_control.h"
#include "cubic_engine/ml/datasets/data_set_loaders.h"
#include "cubic_engine/ml/datasets/blaze_regression_dataset.h"
#include "kernel/maths/lp_metric.h"
#include "kernel/utilities/csv_file_writer.h"

#include <iostream>


int main(){

    using cengine::uint_t;
    using cengine::real_t;
    using cengine::DynMat;
    using cengine::DynVec;
    using cengine::ml::KnnControl;
    using cengine::ml::KnnClassifier;
    using cengine::ml::BlazeRegressionDataset;
    using kernel::LpMetric;
    using kernel::utilities::CSVWriter;

    typedef LpMetric<2> similarity_t;


    try{

        BlazeRegressionDataset dataset;
        {
            auto features = cengine::ml::load_iris_data_set(false);
            dataset.load_from_data(features.first, features.second);
        }

        KnnClassifier<BlazeRegressionDataset, similarity_t> knn(KnnControl(4));
        knn.fit(dataset);

        // for every row predict the label
        auto result = knn.predict_many(dataset);
        std::cout<<result<<std::endl;

        auto& predictions = result;

        {
            CSVWriter writer("knn_classifier_" + std::to_string(4), ',', true);

            std::vector<std::string> names(dataset.n_features() + 1);
            names[0] = "ClusterId";

            for(uint_t i=1; i<names.size(); ++i){
                names[i] = "X-"+std::to_string(i);
            }

            //write the names
            writer.write_row(names);

            std::vector<real_t> data_row(names.size());

            for(uint_t i=0; i<predictions.size(); ++i){

                auto prediction = predictions[i];
                data_row[0] = prediction;
                auto row = dataset.get_row(i);

                for(uint_t r=0; r<row.size(); ++r){
                    data_row[r+1] = row[r];
                }

                writer.write_row(data_row);
            }
        }

        {
            CSVWriter writer("knn_classifier_exact" , ',', true);

            std::vector<std::string> names(dataset.n_features() + 1);
            names[0] = "ClusterId";

            for(uint_t i=1; i<names.size(); ++i){
                names[i] = "X-"+std::to_string(i);
            }

            //write the names
            writer.write_row(names);

            std::vector<real_t> data_row(names.size());

            for(uint_t i=0; i<dataset.n_examples(); ++i){

                auto class_idx = dataset.get_label(i);
                data_row[0] = class_idx;
                auto row = dataset.get_row(i);

                for(uint_t r=0; r<row.size(); ++r){
                    data_row[r+1] = row[r];
                }

                writer.write_row(data_row);
            }
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


#include "cubic_engine/base/cubic_engine_types.h"
#include "cubic_engine/ml/instance_learning/threaded_knn.h"
#include "cubic_engine/ml/instance_learning/utils/knn_control.h"
#include "cubic_engine/ml/instance_learning/utils/knn_classification_policy.h"
#include "cubic_engine/ml/instance_learning/utils/knn_info.h"

#include "kernel/utilities/data_set_loaders.h"
#include "kernel/maths/lp_metric.h"
#include "kernel/utilities/csv_file_writer.h"
#include "kernel/parallel/utilities/partitioned_type.h"
#include "kernel/parallel/threading/thread_pool.h"
#include <iostream>


int main(){

    using cengine::uint_t;
    using cengine::real_t;
    using cengine::DynMat;
    using cengine::DynVec;
    using cengine::KnnControl;
    using cengine::KnnClassificationPolicy;
    using cengine::ThreadedKnn;
    using cengine::Null;
    using kernel::LpMetric;
    using kernel::CSVWriter;
    using kernel::PartitionedType;
    using kernel::ThreadPool;

    typedef LpMetric<2> similarity_t;
    typedef KnnClassificationPolicy actor_t;

    try{

        const uint_t N_THREADS = 4;
        ThreadPool executor(N_THREADS);

        auto dataset = kernel::load_iris_data_set_with_partitions(4, false);

        ThreadedKnn<PartitionedType<DynMat<real_t>>,
                    PartitionedType<DynVec<uint_t>>, similarity_t, actor_t> knn(KnnControl(4));

        knn.train(dataset.first, dataset.second);
        //auto result = knn.predict(dataset.first, executor, Null());
        //std::cout<<result.second<<std::endl;

        /*auto& predictions = result.first;

        {
            kernel::CSVWriter writer("knn_classifier_" + std::to_string(result.second.n_neighbors),
                                     kernel::CSVWriter::default_delimiter(), true);

            std::vector<std::string> names(dataset.first.columns() + 1);
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
                auto row = kernel::get_row(dataset.first, i);

                for(uint_t r=0; r<row.size(); ++r){
                    data_row[r+1] = row[r];
                }

                writer.write_row(data_row);
            }
        }*/

        {
            kernel::CSVWriter writer("iri_data_set_parition" ,
                                     kernel::CSVWriter::default_delimiter(), true);

            std::vector<std::string> names(dataset.first.columns() + 1);
            names[0] = "PartitionId";

            for(uint_t i=1; i<names.size(); ++i){
                names[i] = "X-"+std::to_string(i);
            }

            //write the names
            writer.write_row(names);

            std::vector<real_t> data_row(names.size());

            uint_t n_partitions = dataset.first.n_partitions();

            for(uint_t partition=0; partition < n_partitions; ++partition){

                data_row[0] = partition;
                auto range = dataset.first.get_partition(partition);

                for(uint_t start=range.begin(); start<range.end(); ++start){

                    auto row = kernel::get_row(dataset.first, start);

                    for(uint_t r=0; r<row.size(); ++r){
                        data_row[r+1] = row[r];
                    }

                    writer.write_row(data_row);
                }
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

#include "cubic_engine/base/cubic_engine_types.h"
#include "cubic_engine/ml/datasets/blaze_regression_dataset.h"
#include "cubic_engine/ml/datasets/data_set_loaders.h"

#include <vector>
#include <stdexcept>
#include <gtest/gtest.h>

namespace {
using cengine::DynMat;
using cengine::DynVec;
using cengine::real_t;
using cengine::uint_t;
using cengine::ml::BlazeRegressionDataset;

struct TestSetLoader{

    template<typename ColsTp>
    void load(DynMat<real_t>& mat, DynVec<real_t>& labels, ColsTp& columns) const;
};

template<typename ColsTp>
void
TestSetLoader::load(DynMat<real_t>& mat, DynVec<real_t>& labels, ColsTp& /*columns*/)const{

    cengine::ml::load_car_plant_multi_dataset(mat, labels, 2, false);
}

}


TEST(TestBlazeRegressionDataset, Empty) {

    try{

        BlazeRegressionDataset dataset;

        ASSERT_TRUE(dataset.empty());
        ASSERT_EQ(dataset.n_features(), 0);
        ASSERT_EQ(dataset.n_examples(), 0);

    }
    catch(...){

        ASSERT_FALSE("A non expected exception was thrown");
    }
}



TEST(TestBlazeRegressionDataset, LoadFromLoader) {

    try{

        BlazeRegressionDataset dataset;

        TestSetLoader loader;
        dataset.load_from_loader(loader);

        ASSERT_FALSE(dataset.empty());
        ASSERT_EQ(dataset.n_features(), 2);
        ASSERT_EQ(dataset.n_examples(), 12);

    }
    catch(std::invalid_argument& exception){
        ASSERT_FALSE(exception.what());
    }
    catch(...){

        ASSERT_FALSE("A non expected exception was thrown");
    }
}


TEST(TestBlazeRegressionDataset, Iterate) {

    try{

        BlazeRegressionDataset dataset;

        DynMat<real_t> mat(10, 1, 1.0);
        DynVec<real_t> vec(10, 1.0);
        dataset.load_from_data(mat, vec);

        ASSERT_FALSE(dataset.empty());
        ASSERT_EQ(dataset.n_features(), 1);
        ASSERT_EQ(dataset.n_examples(), 10);

        real_t rows_result = 0.0;
        real_t labels_result = 0.0;

        auto summer = [&rows_result, &labels_result](const auto& row, const auto& vec){
            rows_result += row[0];
            labels_result += vec;
        };

        dataset.iterate(summer);
        ASSERT_DOUBLE_EQ(rows_result, 10.);
        ASSERT_DOUBLE_EQ(labels_result, 10.);

    }
    catch(std::invalid_argument& exception){
        ASSERT_FALSE(exception.what());
    }
    catch(...){

        ASSERT_FALSE("A non expected exception was thrown");
    }
}


TEST(TestBlazeRegressionDataset, Accumulate) {

    try{

        BlazeRegressionDataset dataset;

        const uint_t SIZE = 3;

        DynMat<real_t> mat(SIZE, 1, 1.0);
        DynVec<real_t> vec(SIZE, 1.0);
        dataset.load_from_data(mat, vec);

        ASSERT_FALSE(dataset.empty());
        ASSERT_EQ(dataset.n_features(), 1);
        ASSERT_EQ(dataset.n_examples(), SIZE);

        auto row_identity = [](const auto& row, const auto&){
            return row[0];
        };

        auto result = dataset.accumulate(row_identity, dataset.n_examples());
        ASSERT_EQ(result.size(), SIZE);

        for(uint_t i=0; i<SIZE; ++i){
            ASSERT_DOUBLE_EQ(result[i], 1.);
        }
    }
    catch(std::invalid_argument& exception){
        ASSERT_FALSE(exception.what());
    }
    catch(...){

        ASSERT_FALSE("A non expected exception was thrown");
    }
}











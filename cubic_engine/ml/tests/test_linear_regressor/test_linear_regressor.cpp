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












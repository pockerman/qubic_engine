#include "cubic_engine/ml/datasets/data_set_loaders.h"
#include "cubic_engine/base/cubic_engine_types.h"
#include "kernel/base/exceptions.h"
#include "kernel/maths/matrix_utilities.h"

#include <vector>
#include <stdexcept>
#include <gtest/gtest.h>

namespace {
using cengine::DynMat;
using cengine::DynVec;
using cengine::real_t;
}

/// 
/// Test Scenario:   The application attempts to load the reduced iris data set
/// Expected Output:	Data set should be loaded
///
TEST(TestDataSetLoaders, LoadReducedIrisSet) {


    // TODO: Is there a better way to do this?
    try{

        auto data = cengine::ml::load_reduced_iris_data_set();
        ASSERT_EQ(data.first.rows(), 100);
        ASSERT_EQ(data.first.columns(), 5);
        ASSERT_EQ(data.second.size(), data.first.rows());

    }
    catch(...){

        ASSERT_FALSE("A non expected exception was thrown");
    }
}

/***
 * Test Scenario:   The application attempts to load the iris data set without ones
 * Expected Output:	Data set should be loaded
 **/

TEST(TestDataSetLoaders, LoadIrisSetNoOnes) {


    // TODO: Is there a better way to do this?
    try{

        auto data = cengine::ml::load_iris_data_set(false);
        auto expected_rows = 150;
        auto expected_columns = 4;
        ASSERT_EQ(data.first.rows(), expected_rows);
        ASSERT_EQ(data.first.columns(), expected_columns);
        ASSERT_EQ(data.second.size(), data.first.rows());

    }
    catch(...){

        ASSERT_FALSE("A non expected exception was thrown");
    }
}

/***
 * Test Scenario:   The application attempts to load the iris data set with ones column
 * Expected Output:	Data set should be loaded
 **/

TEST(TestDataSetLoaders, LoadIrisSetWithOnes) {

    // TODO: Is there a better way to do this?
    try{

        auto data = cengine::ml::load_iris_data_set(true);
        auto expected_rows = 150;
        auto expected_columns = 5;
        ASSERT_EQ(data.first.rows(), expected_rows);
        ASSERT_EQ(data.first.columns(), expected_columns);
        ASSERT_EQ(data.second.size(), data.first.rows());
    }
    catch(...){

        ASSERT_FALSE("A non expected exception was thrown");
    }
}

///
/// Test Scenario:   The application attempts to load the x_y sinuisoid iris data set
/// Expected Output:	Data set should be loaded
///
TEST(TestDataSetLoaders, LoadXYSinuisoidDataSet) {


    /// TODO: Is there a better way to do this?
    try{

        auto data = cengine::ml::load_x_y_sinuisoid_data_set();
        ASSERT_EQ(data.first.rows(), 20);
        ASSERT_EQ(data.first.columns(), 2);
        ASSERT_EQ(data.second.size(), data.first.rows());

    }
    catch(...){

        ASSERT_FALSE("A non expected exception was thrown");
    }
}

///
/// Test Scenario:   The application attempts to load the x_y sinuisoid iris data set
/// Expected Output:	Data set should be loaded
///
TEST(TestDataSetLoaders, LoadXYSinuisoidDataSetNoOnes) {


    /// TODO: Is there a better way to do this?
    try{

        auto data = cengine::ml::load_x_y_sinuisoid_data_set(false);
        ASSERT_EQ(data.first.rows(), 20);
        ASSERT_EQ(data.first.columns(), 1);
        ASSERT_EQ(data.second.size(), data.first.rows());

    }
    catch(...){

        ASSERT_FALSE("A non expected exception was thrown");
    }
}


/// 
/// Test Scenario:   Test loading the wine data set without ones
/// Expected Output:	matrix of 178x12, labels of 178
///
TEST(TestDataSetLoaders, TestLoadWineDataSetSetNoOnes) {


    // TODO: Is there a better way to do this?
    try{

        auto data = cengine::ml::load_wine_data_set(false);
        ASSERT_EQ(data.first.rows(), 178);
        ASSERT_EQ(data.first.columns(), 12);

        // test first row: 14.23,1.71,2.43,15.6,127,2.8,3.06,.28,2.29,5.64,1.04,3.92,1065

        auto row = kernel::get_row(data.first, 0);
        std::vector<real_t> first_vals{14.23,1.71,2.43,15.6,127,2.8,3.06,.28,2.29,5.64,1.04,3.92,1065};

        for(auto c=0; c<row.size(); ++c){
            auto val = row[c];
            ASSERT_NEAR(val, first_vals[c], 1.0e-10);
        }

        // test last row: 14.13,4.1,2.74,24.5,96,2.05,.76,.56,1.35,9.2,.61,1.6,560
        row = kernel::get_row(data.first, data.first.rows()-1);
        std::vector<double> last_vals{14.13,4.1,2.74,24.5,96,2.05,.76,.56,1.35,9.2,.61,1.6,560};

        for(auto c=0; c<row.size(); ++c){
            auto val = row[c];
            ASSERT_NEAR(val, last_vals[c], 1.0e-10);
        }

        ASSERT_EQ(data.second.size(), data.first.rows());

        // there should be:
        // class 1 59
        // class 2 71
        // class 3 48
        auto class_1 = std::count(data.second.begin(),
                                  data.second.end(), 1);

        ASSERT_EQ(class_1, 59);

        auto class_2 = std::count(data.second.begin(),
                                  data.second.end(), 2);

        ASSERT_EQ(class_2, 71);

        auto class_3 = std::count(data.second.begin(),
                                  data.second.end(), 3);

        ASSERT_EQ(class_3, 48);
    }
    catch(...){

        ASSERT_FALSE("A non expected exception was thrown");
    }
}

/***
 * Test Scenario:   Test loading the wine data set without ones
 * Expected Output:	matrix of 178x12, labels of 178
 **/
TEST(TestDataSetLoaders, TestLoadWineDataSetSetWithOnes) {


    // TODO: Is there a better way to do this?
    try{

        auto data = cengine::ml::load_wine_data_set(true);
        ASSERT_EQ(data.first.rows(), 178);
        ASSERT_EQ(data.first.columns(), 13);
        ASSERT_EQ(data.second.size(), data.first.rows());

        // there should be:
        // class 1 59
        // class 2 71
        // class 3 48
        auto class_1 = std::count(data.second.begin(),
                                  data.second.end(), 1);

        ASSERT_EQ(class_1, 59);

        auto class_2 = std::count(data.second.begin(),
                                  data.second.end(), 2);

        ASSERT_EQ(class_2, 71);

        auto class_3 = std::count(data.second.begin(),
                                  data.second.end(), 3);

        ASSERT_EQ(class_3, 48);
    }
    catch(...){

        ASSERT_FALSE("A non expected exception was thrown");
    }
}

TEST(TestDataSetLoaders, TestRandomSetOne){
	
	
	try{
		
                DynMat<real_t> data;

                cengine::ml::load_random_set_one(data);
                ASSERT_EQ(data.rows(), 100);
                ASSERT_EQ(data.columns(), 2);
    }
	catch(std::logic_error& e){
		ASSERT_TRUE("Invalid line size");
	}
    catch(...){

        ASSERT_FALSE("A non expected exception was thrown");
    }
	
	
}











#include "cubic_engine/base/cubic_engine_types.h"
#include "kernel/utilities/csv_file_writer.h"
#include "kernel/maths/statistics/multivariate_normal_distribution.h"

#include <iostream>

namespace example{
using cengine::uint_t;
using cengine::real_t;
using cengine::DynVec;
using cengine::DynMat;
using kernel::maths::stats::MultiNormalDist;
using kernel::CSVWriter;
}


int main(){

    using namespace example;

    try{

        {
            DynVec<real_t> mu(2, 0.0);
            mu[0] = 1.0;
            mu[1] = 2.0;
            DynMat<real_t> sigma(2,2, 0.0);
            sigma(0,0) = 1.0;
            sigma(1,1) = 4.0;
            sigma(0, 1) = 0.6;
            sigma(1, 0) = 0.6;

            MultiNormalDist n(mu, sigma);

            CSVWriter w("sample_1.csv", ',', true);
            std::vector<std::string> names{"X1", "X2"};
            w.write_column_names(names);

            for(uint_t itr=0; itr<1000; ++itr){

                auto point = n.sample();
                w.write_row(point);
            }
        }

        {
            DynVec<real_t> mu(2, 0.0);
            DynMat<real_t> sigma(2,2, 0.0);
            sigma(0,0) = 1.0;
            sigma(1,1) = 2.0;

            MultiNormalDist n(mu, sigma);

            CSVWriter w("sample_2.csv", ',', true);
            std::vector<std::string> names{"X1", "X2"};
            w.write_column_names(names);

            for(uint_t itr=0; itr<1000; ++itr){

                auto point = n.sample();
                w.write_row(point);
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

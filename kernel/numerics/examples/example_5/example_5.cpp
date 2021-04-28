#include "kernel/base/types.h"
#include "kernel/base/kernel_consts.h"
#include "kernel/utilities/csv_file_writer.h"

#include <random>
#include <iostream>
#include <vector>
#include <string>


int main(){

    using kernel::real_t;
    using kernel::uint_t;
    using kernel::utilities::CSVWriter;


    // the lambda coeff
    const real_t LAMBDA = 0.13149;

    // time limit for simulation
    const real_t TIME_LIMIT = 30.0;

    // time increment
    const real_t TIME_INCREMENT = 0.001/LAMBDA;

    const real_t PROB_UNIT_TIME=0.001*LAMBDA;

    std::random_device rd;  //Will be used to obtain a seed for the random number engine
    std::mt19937 gen(rd());

    // real number generator in [0, 4.0*PROB_UNIT_TIME]
    std::uniform_real_distribution<real_t> dist(0.0, 4.0*PROB_UNIT_TIME);

    // number of nuclei in the sample
    uint_t n_nuclei = 100;

    real_t time = 0.0;

    bool stop_before_time = false;

    CSVWriter file_writer("radio_active_decay.csv");
    file_writer.open(std::ios_base::out);
    file_writer.write_column_names(std::vector<std::string>({"Time", "Nuclei"}));

    std::vector<real_t> row(2);

    while(time <= TIME_LIMIT ){

        row[0] = time;
        row[1] = static_cast<real_t>(n_nuclei);
        file_writer.write_row(row);

         time += TIME_INCREMENT;

        // loop over the nuclei
        for(uint_t i = 0; i <n_nuclei; ++i){

            auto num = dist(gen);

            if(num <= PROB_UNIT_TIME){

                n_nuclei -= 1;

                if(n_nuclei == 0){
                    stop_before_time = true;
                    break; // no more nucleis in the sample
                }
            }
        }

        if(stop_before_time){
            break;
        }
    }

    return 0;
}




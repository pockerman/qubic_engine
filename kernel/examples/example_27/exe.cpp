#include "kernel/base/types.h"
#include <cmath>
#include <iostream>
#include <array>
#include <random>

namespace example
{
using kernel::real_t;
using kernel::uint_t;
}

int main(){

    using namespace example;

    try{

        std::array<std::string, 3> doors;
        doors[0] = "A";
        doors[1] = "B";
        doors[2] = "C";

        uint_t first_choice_wins = 0;
        uint_t change_wins = 0;

        const uint_t N_ITRS = 1000;

        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<uint_t> distribution(0, 2);

        for(uint_t itr=0; itr<N_ITRS; ++itr){

            auto winner_idx = distribution(gen);
            auto pick_idx = distribution(gen);

            std::cout<<"You chose: "<<doors[pick_idx]<<" Winner door: "
                    <<doors[winner_idx]<<std::endl;

            if(winner_idx == pick_idx){
                first_choice_wins++;
            }
            else{
                change_wins++;
            }
        }

        std::cout<<"Wins with original choice: "<<first_choice_wins<<std::endl;
        std::cout<<"Wins with change choice: "<<change_wins<<std::endl;
        std::cout<<"Probability of winning with initial guess: "<<static_cast<real_t>(first_choice_wins)/N_ITRS<<std::endl;
        std::cout<<"Probability of winning with change guess: "<<static_cast<real_t>(change_wins)/N_ITRS<<std::endl;
    }
    catch(std::logic_error& error){

        std::cerr<<error.what()<<std::endl;
    }
    catch(...){
        std::cerr<<"Unknown exception occured"<<std::endl;
    }

    return 0;
}






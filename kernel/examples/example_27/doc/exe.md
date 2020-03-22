# Example 27: Monte Carlo Simulation for the Monty Hall Problem

## Contents

* [Acknowledgements](#acknowledgements) 
* [Overview](#overview) 
* [Include files](#include_files)
* [The main function](#m_func)
* [Results](#results)
* [Source Code](#source_code)

## <a name="acknowledgements"></a> Acknowledgements
This example was taken from Lee Vaughan's book _Impractical Python Projects_.
## <a name="overview"></a> Overview

## <a name="include_files"></a> Include files

```
#include "kernel/base/types.h"
#include <cmath>
#include <iostream>
#include <array>
#include <random>

```

## <a name="m_func"></a> The main function

```
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
    catch(...){
        std::cerr<<"Unknown exception occured"<<std::endl;
    }

    return 0;
}
```

## <a name="results"></a> Results

```
...
You chose: A Winner door: A
You chose: C Winner door: C
You chose: B Winner door: A
You chose: C Winner door: A
You chose: C Winner door: B
You chose: C Winner door: A
You chose: A Winner door: A
You chose: B Winner door: B
You chose: B Winner door: C
You chose: C Winner door: B
You chose: B Winner door: B
You chose: C Winner door: C
You chose: B Winner door: C
You chose: B Winner door: B
You chose: A Winner door: A
You chose: B Winner door: A
You chose: C Winner door: A
You chose: C Winner door: A
You chose: C Winner door: B
You chose: B Winner door: A
Wins with original choice: 320
Wins with change choice: 680
Probability of winning with initial guess: 0.32
Probability of winning with change guess: 0.68
```

## <a name="source_code"></a> Source Code

<a href="../exe.cpp">exc.cpp</a>





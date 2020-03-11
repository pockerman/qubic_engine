#include "cubic_engine/base/cubic_engine_types.h"
#include "kernel/utilities/csv_file_writer.h"
#include "cubic_engine/rl/worlds/cliff_world.h"


#include <cmath>
#include <utility>
#include <tuple>
#include <iostream>
#include <random>
#include <algorithm>

namespace exe
{

using cengine::uint_t;
using cengine::real_t;
using cengine::DynMat;

// enumeration describing the action space
enum class Action{actionNorth=0,
                  actionSouth=1,
                  actionEast=2,
                  actionWest=3};

// the size of the action space
constexpr int ACTION_SPACE_SIZE = 4;

}

int main(int argc, char** argv) {

   using namespace exe;






    
   return 0;
}


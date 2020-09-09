#include "kernel/maths/optimization/utils/gd_info.h"

namespace kernel{
namespace maths {
namespace opt {



std::ostream&
GDInfo::print(std::ostream& out)const{

  this->AlgInfo::print(out);
  out<<"Learning rate:.."<<learning_rate<<std::endl;
  return out;
}

}
}
}

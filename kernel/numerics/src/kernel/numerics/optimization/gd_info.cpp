#include "kernel/numerics/optimization/gd_info.h"

namespace kernel{
namespace numerics {
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

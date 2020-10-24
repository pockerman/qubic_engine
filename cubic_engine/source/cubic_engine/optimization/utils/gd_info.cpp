#include "cubic_engine/optimization/utils/gd_info.h"

namespace cengine
{

std::ostream&
GDInfo::print(std::ostream& out)const{

  this->AlgInfo::print(out);
  out<<"Learning rate:.."<<learning_rate<<std::endl;
  return out;
}

}

#include "parml/instance_learning/knn_info.h"

namespace parml
{
    
  KnnInfo::KnnInfo()
          :
          nthreads(1),
          nprocs(1),
          type(knn_type::Type::INVALID_TYPE),
          runtime(),
          n_pts_predicted(0),
          n_neighbors(0)
  {}
  
  std::ostream& 
  KnnInfo::print(std::ostream& out)const{
      
      out<<"# threads: "<<nthreads<<std::endl;
      out<<"# procs: "<<nprocs<<std::endl;
      out<<"# points predicted: "<<n_pts_predicted<<std::endl;
      out<<"# neighbors: "<<n_neighbors<<std::endl;
      out<<"Type: "<<knn_type::type_to_string(type)<<std::endl;
      out<<"Total runtime: "<<runtime.count()<<std::endl;
      return out;
  }
    
    
}

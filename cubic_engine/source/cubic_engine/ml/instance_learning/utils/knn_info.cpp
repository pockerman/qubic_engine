#include "cubic_engine/ml/instance_learning/utils/knn_info.h"

namespace cengine
{
    
  KnnInfo::KnnInfo()
          :
          nthreads(1),
          nprocs(1),
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
      out<<"Total runtime: "<<runtime.count()<<std::endl;
      return out;
  }
    
    
}

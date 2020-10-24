#include "parml/instance_learning/knn_mpi.h"

#ifdef PARML_HAS_MPI

namespace parml
{

Knn<knn_type::Type::MPI_KNN>::Knn(parframepp::data_dist_type::Type t)
        :
        data_dist_type_flag_(t)
{}
    
    
}

#endif

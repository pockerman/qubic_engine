#include "parml/algorithms/mpi_batch_gradient_descent.h"

#ifdef PARML_HAS_MPI

namespace parml
{
  
    Gd<gradient_descent_type::Type::MPI_BGD/*mpi_par,batch_gd*/>::Gd(const GDControl& data)
            :
    parframepp::par_indexed_object(),
    data_(data)
    {}
    
}

#endif

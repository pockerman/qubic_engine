#include "robosim/grids/cell_vertex.h"

namespace robosim
{
    
 CellVertex::CellVertex(size_type idx)
 :
 utilities::indexed_obj(idx),
 position_()
 {}   
    
CellVertex::CellVertex(const arma::vec& position,size_type idx)
        :
utilities::indexed_obj(idx),
position_(position)
{}       
    
}

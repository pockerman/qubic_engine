#include "robosim/grids/base_grid.h"
#include "robosim/grids/base_grid_cell.h"
#include "robosim/grids/cell_vertex.h"
#include "utilities/array_utilities.h"

#ifdef ROBOSIM_HAS_OPENGL
#include "glut.h"
#endif


namespace robosim
{
    
BaseGrid::BaseGrid()
:
cells_(),
vertices_()
{}

BaseGridCell* 
BaseGrid::add_cell(){
    
    //how many cells the grid has
    size_type id = cells_.size();
    
    BaseGridCell* ptr = new BaseGridCell(id,this);
    cells_.push_back(ptr);
    return ptr;
}




CellVertex* 
BaseGrid::add_vertex(){
    
   //how many cells the grid has
    size_type id = vertices_.size();
    
    CellVertex* ptr = new CellVertex(id);
    vertices_.push_back(ptr);
    return ptr; 
}
     
     
CellVertex* 
BaseGrid::add_vertex(arma::vec&& pos){
   
    CellVertex* v = add_vertex();
    v->set_position(pos);
    pos.clear();
    return v;
    
}
     
    
CellVertex* 
BaseGrid::add_vertex(std::initializer_list<real_type>&& pos){
  
    CellVertex* v = add_vertex();
    v->set_position(pos);
    //pos.clear();
    return v;   
}


void 
BaseGrid::clear(){
    utilities::clear_pointer_array(cells_);
    utilities::clear_pointer_array(vertices_);
}

#ifdef ROBOSIM_HAS_OPENGL


void 
BaseGrid::render(){
    
    for(size_type c=0; c<cells_.size(); ++c)
        cells_[c]->render();
    
}
#endif
     
        
    
    
}

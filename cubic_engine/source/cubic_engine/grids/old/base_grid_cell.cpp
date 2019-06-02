#include "robosim/grids/base_grid_cell.h"
#include "robosim/grids/cell_vertex.h"
#include "robosim/grids/base_grid.h"

#ifdef ROBOSIM_HAS_OPENGL
#include "glut.h"
#endif

namespace robosim
{
    
   BaseGridCell::BaseGridCell(size_type id,const BaseGrid* grid)
           :
    utilities::indexed_obj(id),
    neighbors_(),
    oc_prob_(0.0),
    grid_ptr_(grid)
   {}
   
   
   bool 
   BaseGridCell::is_occupied(real_type tol)const{
        
        if(oc_prob_ -tol > 0.0 )
            return true;
        
        return false;
    }
    
    
    
    const arma::vec 
    BaseGridCell::get_centroid()const{
        
/*#ifdef ROBOSIM_DEBUG
        using utilities::ExeLogicError;
        const std::string msg="In BaseGridCell::get_centroid(). Empty vertices list.";
        Assert(vertices_.empty()==false,ExeLogicError(msg));
#endif*/
        
        //initialize with the position of the first vertex
        arma::vec center = grid_ptr_->vertex(vertices_[0])->get_position();
        
        for(size_type v=1; v <vertices_.size(); ++v){
            const CellVertex* vertex = grid_ptr_->vertex(vertices_[v]);
            center += vertex->get_position();   
        }
        
        center /= vertices_.size();
        return center;
    }
    
    
    std::ostream& 
    BaseGridCell::print(std::ostream& out)const{
        
        out<<"Id: "<<this->get_id()<<std::endl;
        out<<"Occupation prob: "<<oc_prob_<<std::endl;
        
        for(size_type n=0; n < neighbors_.size(); ++n){
            
            out<<"Neighbor: "<<n<<" "<<neighbors_[n]<<std::endl;
        }
        
        for(size_type v=0; v < vertices_.size(); ++v){
            
          out<<"Vertex: "<<v<<" "<<vertices_[v]<<std::endl;  
        }
        
        return out;
    }
    
#ifdef ROBOSIM_HAS_OPENGL
    void 
    BaseGridCell::render(){
        
        this->PolyRenderer::render();
        
        float z = 0.0f;
        glBegin(GL_LINE_LOOP);
        
            const CellVertex* v = grid_ptr_->vertex(vertices_[0]);
            const arma::vec& pos0 = v->get_position();
            glVertex3f(static_cast<float>(pos0(0)), static_cast<float>(pos0(1)) , z);
            
            v = grid_ptr_->vertex(vertices_[1]);
            const arma::vec& pos1 = v->get_position();
            glVertex3f(static_cast<float>(pos1(0)), static_cast<float>(pos1(1)) , z);
        
            v = grid_ptr_->vertex(vertices_[2]);
            const arma::vec& pos2 = v->get_position();
            glVertex3f(static_cast<float>(pos2(0)), static_cast<float>(pos2(1)) , z);
            
            v = grid_ptr_->vertex(vertices_[3]);
            const arma::vec& pos3 = v->get_position();
            glVertex3f(static_cast<float>(pos3(0)), static_cast<float>(pos3(1)) , z);
    
        glEnd();
        
        
    }
#endif
}

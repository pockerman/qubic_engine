#include "robosim/plans/location_plan.h"
#include "utilities/file_writer_csv.h"

namespace robosim
{
    
 LocationPlan2D::LocationPlan2D()
         :
         start_(),
         goal_(),
         locations_()
 {}
    
    
LocationPlan2D::LocationPlan2D(const arma::vec& start,const arma::vec& goal)
        :
         start_(start),
         goal_(goal),
         locations_()
 {}


void
LocationPlan2D::save(const std::string& filename){
    
    utilities::file_writer_csv writer(filename);
    writer.open();
    
    //ouput the locations
    for(size_type l=0; l< locations_.size(); ++l){
      
        writer.write_row(locations_[l]);
        
    }
    
    writer.close();
}
    
}

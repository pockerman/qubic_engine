#include "kernel/utilities/csv_file_writer.h"
#include "kernel/discretization/line_mesh.h"
#include "kernel/discretization/node.h"
#include "kernel/discretization/node_mesh_iterator.h"
#include "kernel/discretization/mesh_predicates.h"

namespace kernel
{

CSVWriter::CSVWriter(const std::string& filename, char delim, bool open_file, const std::ios_base::openmode mode)
                :
FileWriterBase(filename, FileFormats::Type::CSV, open_file, mode),
delim_(delim)
{}

void
CSVWriter::write_column_names(const std::vector<std::string>& col_names){

    //if the file is not open
    if(!is_open()){
        throw std::logic_error("File "+this->file_name_+" is not open");
    }

    for(uint_t c=0; c<col_names.size(); ++c){

        if(c==0){
          this->file_<<"#"+col_names[c]<<",";
        }
        else{
           this->file_<<col_names[c]<<",";
        }

        if(c == col_names.size()-1){
            this->file_<<std::endl;
        }
    }
}

void
CSVWriter::write_mesh(const numerics::LineMesh<2>& mesh ){

    ///if the file is not open throw
    if(!is_open()){
        throw std::logic_error("File "+this->file_name_+" is not open");
    }

    numerics::ConstNodeMeshIterator<numerics::Active, numerics::LineMesh<2>> filter(mesh);

    auto begin = filter.begin();
    auto end = filter.end();

    for(; begin != end; ++begin){
        auto* node  = *begin;

        this->file_<<(*node)[0]<<","<<(*node)[1]<<std::endl;
    }

    this->file_.flush();
    this->file_.close();

}

}

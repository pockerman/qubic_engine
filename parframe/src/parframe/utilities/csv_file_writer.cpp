#include "parframe/utilities/csv_file_writer.h"

namespace kernel
{

CSVWriter::CSVWriter(const std::string& filename, char delim)
                :
FileWriterBase(filename, FileFormats::Type::CSV),
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
}

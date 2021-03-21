#include "kernel/utilities/csv_file_writer.h"


namespace kernel{
namespace utilities {

CSVWriter::CSVWriter(const std::string& filename, char delim, bool open_file, const std::ios_base::openmode mode)
                :
FileWriterBase(filename, FileFormats::Type::CSV, open_file, mode),
delim_(delim)
{}

void
CSVWriter::write_column_names(const std::vector<std::string>& col_names, bool write_header){

    //if the file is not open
    if(!is_open()){
        throw std::logic_error("File "+this->file_name_+" is not open");
    }

    if(write_header){
        this->write_header();
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
CSVWriter::write_column_names(const std::vector<std::string_view>& col_names, bool write_header){

    //if the file is not open
    if(!is_open()){
        throw std::logic_error("File "+this->file_name_+" is not open");
    }

    if(write_header){
        this->write_header();
    }

    for(uint_t c=0; c<col_names.size(); ++c){

        if(c==0){
          this->file_<<"#"+std::string(col_names[c])<<",";
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
CSVWriter::write_column_names(const std::initializer_list<std::string_view>& col_names, bool write_header){

    std::vector<std::string_view> names(col_names);
    write_column_names(names, write_header);
}

}
}

#ifndef CSV_FILE_WRITER_H
#define CSV_FILE_WRITER_H

#include "kernel/base/types.h"
#include "kernel/utilities/file_writer_base.h"
#include <vector>

namespace kernel
{

/**
  * \brief Handles writing into CSV file format
  */
class CSVWriter: public FileWriterBase
{

public:

    /**
     * @brief The default column delimiter
     */
    static char default_delimiter(){return ',';}

    /**
     * @brief Constructor
     */
    CSVWriter(const std::string& filename, char delim=CSVWriter::default_delimiter(), bool open_file=false, const std::ios_base::openmode mode=std::ios_base::out);

    /**
     * @brief Write the column names
     */
    void write_column_names(const std::vector<std::string>& col_names);

    /**
     * @brief Write a row of the file
     */
    template<typename T>
    void write_row(const std::vector<T>& vals);

    /**
     * @brief Write the given Vec as a row
     */
    template<typename T>
    void write_row(const DynVec<T>& vals);

    /**
     * @brief Set the delimiter
     */
    void set_delimiter(char delim)noexcept{delim_=delim;}


    /**
     * @brief Returns the column delimiter
     */
    char get_delimiter()const noexcept{return delim_;}

private:

    /**
     * @brief The delimiter used to separate the values
     */
    char delim_;

};

template<typename T>
void
CSVWriter::write_row(const std::vector<T>& vals){

    if(!is_open()){
        throw std::logic_error("File "+this->file_name_+" is not open");
    }

    for(uint_t c=0; c<vals.size(); ++c){

        file_<<vals[c]<<",";

        if(c == vals.size()-1){
            file_<<std::endl;
        }
    }
}

template<typename T>
void
CSVWriter::write_row(const DynVec<T>& vals){

    //if the file is not open
    if(!is_open()){
        throw std::logic_error("File "+this->file_name_+" is not open");
    }

    for(uint_t c=0; c<vals.size(); ++c){

        file_<<vals[c]<<",";

        if(c == vals.size()-1){
            file_<<std::endl;
        }
    }

}

}

#endif // CSV_FILE_WRITER_H

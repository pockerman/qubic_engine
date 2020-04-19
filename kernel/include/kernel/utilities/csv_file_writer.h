#ifndef CSV_FILE_WRITER_H
#define CSV_FILE_WRITER_H

#include "kernel/base/types.h"
#include "kernel/utilities/file_writer_base.h"
#include "kernel/discretization/node_mesh_iterator.h"
#include "kernel/discretization/mesh_predicates.h"
#include <vector>

namespace kernel{

namespace numerics {
template<int dim> class LineMesh;
}


/// \brief Handles writing into CSV file format
class CSVWriter: public FileWriterBase
{

public:

    /// \brief  The default column delimiter
    static char default_delimiter(){return ',';}

    /// \brief Constructor
    CSVWriter(const std::string& filename, char delim=CSVWriter::default_delimiter(),
              bool open_file=false, const std::ios_base::openmode mode=std::ios_base::out);

    /// \brief Write the column names
    void write_column_names(const std::vector<std::string>& col_names);

    /// \brief Write a row of the file
    template<typename T>
    void write_row(const std::vector<T>& vals);

    /// \brief Write the given Vec as a row
    template<typename T>
    void write_row(const DynVec<T>& vals);

    /// \brief Set the delimiter
    void set_delimiter(char delim)noexcept{delim_=delim;}

    /// \brief Returns the column delimiter
    char get_delimiter()const noexcept{return delim_;}

    /// \brief Write the given LineMesh into csv format
    void write_mesh(const numerics::LineMesh<2>& mesh );

    /// \brief Write the nodes of the given Mesh
    template<typename MeshTp>
    void write_mesh_nodes(const MeshTp& mesh);

private:

    /// \brief The delimiter used to separate the values
    char delim_;

};

template<typename T>
void
CSVWriter::write_row(const std::vector<T>& vals){

    if(!is_open()){
        throw std::logic_error("File "+this->file_name_+" is not open");
    }

    for(uint_t c=0; c<vals.size(); ++c){

        file_<<vals[c];

        if(c == vals.size()-1){
            file_<<std::endl;
        }
        else{
           file_<<",";
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

        file_<<vals[c];

        if(c == vals.size()-1){
            file_<<std::endl;
        }
        else{
           file_<<",";
        }
    }

}

template<typename MeshTp>
void
CSVWriter::write_mesh_nodes(const MeshTp& mesh){

    ///if the file is not open throw
    if(!is_open()){
        throw std::logic_error("File "+this->file_name_+" is not open");
    }

    numerics::ConstNodeMeshIterator<numerics::Active, MeshTp> filter(mesh);

    auto begin = filter.begin();
    auto end = filter.end();

    for(; begin != end; ++begin){
        auto* node  = *begin;

        for(uint_t d =0; d<MeshTp::dimension; ++d){

           this->file_<<(*node)[d];

            if(d == MeshTp::dimension -1){
                 this->file_<<std::endl;
             }
            else{
                this->file_<<",";
            }
        }
    }

    this->file_.flush();
    this->file_.close();
}


}


#endif // CSV_FILE_WRITER_H

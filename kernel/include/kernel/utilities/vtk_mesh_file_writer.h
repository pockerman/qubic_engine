#ifndef VTK_MESH_FILE_WRITER_H
#define VTK_MESH_FILE_WRITER_H

#include "kernel/utilities/file_writer_base.h"

namespace kernel
{

namespace numerics
{

template<int dim> class Mesh;

class VtkMeshFileWriter: public FileWriterBase
{

public:

    /// \brief Constructor
    VtkMeshFileWriter(const std::string& filename, bool open_file=false);

    /// \brief Write the mesh into the file specified in
    /// constructor of this class
    void write_mesh(const Mesh<2>& mesh);

    /**
     * @brief Write the header of the file. By default some information
     * such as date and time the file was created is written
     */
    virtual void write_header()override;

};


}

}

#endif // VTK_MESH_FILE_WRITER_H

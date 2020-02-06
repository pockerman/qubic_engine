#ifndef VTK_WRITER_H
#define VTK_WRITER_H

#include "kernel/base/config.h"
#include "kernel/utilities/file_writer_base.h"
#include "kernel/utilities/vtk_mesh_file_writer.h"

namespace kernel{
namespace numerics{

template<int dim> class Mesh;
class TrilinosEpetraVector;
template<int dim> class FVDoFManager;


class VtkWriter: public VtkMeshFileWriter
{
public:

    /// \brief Constructor
    VtkWriter(const std::string& filename, bool open_file=false);

#ifdef USE_TRILINOS

    /// \brief Write the mesh into the file specified in
    /// constructor of this class
    void write_solution(const Mesh<1>& mesh, const TrilinosEpetraVector& data, const FVDoFManager<1>& manager);

    /// \brief Write the mesh into the file specified in
    /// constructor of this class
    void write_solution(const Mesh<2>& mesh, const TrilinosEpetraVector& data, const FVDoFManager<2>& manager);
#endif


};
}
}

#endif // VTK_WRITER_H

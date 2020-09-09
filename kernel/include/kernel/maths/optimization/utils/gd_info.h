#ifndef GD_INFO_H
#define GD_INFO_H

#include "kernel/base/types.h"
#include "kernel/utilities/algorithm_info.h"

namespace kernel{
namespace maths {
namespace opt {

///
/// \brief The GDInfo struct. Helper for wrapping
/// up information on the Gradient Descent
///
struct GDInfo: public kernel::AlgInfo
{

    real_t learning_rate;


    /// \brief Print the information related to the application
    ///of gradient descent algorithm to the given stream.
    virtual std::ostream& print(std::ostream& out)const;

};



inline
std::ostream&
operator<<(std::ostream& out,const GDInfo& info){

    return info.print(out);
}

}
}
}

#endif // GD_INFO_H

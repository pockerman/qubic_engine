#ifndef GD_INFO_H
#define GD_INFO_H

#include "cubic_engine/base/cubic_engine_types.h"
#include "kernel/utilities/algorithm_info.h"

namespace cengine
{

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

#endif // GD_INFO_H

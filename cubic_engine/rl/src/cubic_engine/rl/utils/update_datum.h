#ifndef UPDATE_DATUM_H
#define UPDATE_DATUM_H

#include <string>

namespace cengine {
namespace rl {
namespace utils {

template<typename T>
struct UpdateDatum
{
    typedef T value_t;

    std::string name;
    T value;

    ///
    /// \brief UpdateDatum
    ///
    UpdateDatum(std::string n, const T& v)
        :
          name(n),
          value(v)
    {}
};

}

}

}

#endif // UPDATE_DATUM_H

#ifndef INPUT_RESOLVER_H
#define INPUT_RESOLVER_H

#include "boost/any.hpp"
#include <map>
#include <stdexcept>

namespace kernel{
namespace utils{

template<typename InputT, typename OutT>
struct InputResolver;

template<typename OutT>
struct InputResolver<std::map<std::string, boost::any>, OutT>
{
    typedef std::map<std::string, boost::any> input_t;
    typedef OutT out_t;

    static out_t resolve(const std::string& name, const input_t& input);

};

template<typename OutT>
typename InputResolver<std::map<std::string, boost::any>, OutT>::out_t
InputResolver<std::map<std::string, boost::any>, OutT>::resolve(const std::string& name,
                                                                const input_t& input){

    auto itr = input.find(name);

    if(itr == input.end()){
        throw std::logic_error("Property: " + name + " not in input");
    }

    return boost::any_cast<OutT>(itr->second);

}

}
}

#endif // INPUT_RESOLVER_H

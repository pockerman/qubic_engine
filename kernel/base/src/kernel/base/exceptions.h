#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <stdexcept>

namespace kernel
{

/**
 * @brief The InvalidPartitionedObject class. Exception to be
 * thrown when an object does not have the right number of partitions
 */
class InvalidPartitionedObject: public std::logic_error
{
public:

    explicit InvalidPartitionedObject(const std::string& what_arg)
    :
    std::logic_error(what_arg)
    {}

    explicit InvalidPartitionedObject(const char* what_arg)
    :
    std::logic_error(what_arg)
    {}

};
}

#endif // EXCEPTIONS_H

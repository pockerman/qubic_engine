#ifndef SYSTEM_STATE_H
#define SYSTEM_STATE_H

#include "kernel/base/types.h"
#include <vector>
#include <map>
#include <string>
#include <array>
#include <utility>
#include <algorithm>
namespace kernel
{

namespace dynamics
{

/// \brief SysState utility class describing the state of a system
template<int dim>
class SysState
{

public:

    /// \brief Initialize the state with the given names and values
    SysState();

    /// \brief Initialize the state with the given names and values
    SysState(std::array<std::pair<std::string, real_t>, dim>&& values);

    /// \brief Initialize the state with the given names
    /// all variables will be initialized with val
    SysState(std::array<std::string, dim>&& names, real_t val);

    /// \brief Copy constructor
    //SysState(const SysState<dim>& other);

    /// \brief Move copy constructor
    SysState(SysState&& other);

    /// \brief Move copy constructor
    SysState& operator=(SysState&& other);

    /// \brief Returns the value for the variable name
    real_t get(const std::string& name)const;

    /// \brief Returns the value for the variable name
    void set(const std::string& name, real_t val);

    /// \brief Set the name and value of the i-th variable
    void set(uint_t i, const std::pair<std::string, real_t>& value);

    /// \brief Returns the size of the system
    uint_t size()const{return dim;}

    /// \brief Returns a copy of the state values
    std::array<real_t, dim> get_values()const;

    /// \brief Access operators
    real_t& operator[](uint_t);

    /// \brief Access operators
    const real_t& operator[](uint_t)const;

    /// \brief Access operators
    real_t& operator[](const std::string& name);

    /// \brief Access operators
    const real_t& operator[](const std::string& name)const;

    /// \brief clear the state
    void clear();

private:

    std::array<std::pair<std::string, real_t>, dim> values_;

};

template<int dim>
SysState<dim>::SysState()
    :
   values_()
{}

template<int dim>
SysState<dim>::SysState(std::array<std::pair<std::string, real_t>, dim>&& values)
    :
    values_(values)
{}

template<int dim>
SysState<dim>::SysState(std::array<std::string, dim>&& names, real_t val)
    :
   values_()
{
    for(uint_t i=0; i<dim; ++i){
        values_[i] = std::pair(names[i], val);
    }

    names.clear();
}


template<int dim>
SysState<dim>::SysState(SysState&& other)
    :
      values_(other.values_)
{
    other.clear();
}

template<int dim>
SysState<dim>&
SysState<dim>::operator=(SysState&& other){

    if(this == &other){
        return *this;
    }

    this->values_ = other.values_;
    return *this;
}

template<int dim>
real_t
SysState<dim>::get(const std::string& name)const{


    auto itr = std::find(values_.begin(), values_.end(),
                         [&](const std::pair<std::string, real_t>& item){
        return item.first == name;
    });

    if(itr == values_.end()){
        throw std::invalid_argument("Invalid variable name");
    }

    return itr.second;
}

template<int dim>
void
SysState<dim>::set(const std::string& name, real_t val){

    auto itr = std::find(values_.begin(), values_.end(),
                         [&](const std::pair<std::string, real_t>& item){
        return item.first == name;
    });

    if(itr == values_.end()){
        throw std::invalid_argument("Invalid variable name");
    }


    itr->second = val;
}

template<int dim>
void
SysState<dim>::set(uint_t i, const std::pair<std::string, real_t>& value){

    values_[i] = value;

}


template<int dim>
real_t&
SysState<dim>::operator[](uint_t i){
    return values_[i].second;
}

template<int dim>
const real_t&
SysState<dim>::operator[](uint_t i)const{
    return values_[i].second;
}

template<int dim>
real_t&
SysState<dim>::operator[](const std::string& name){

    auto itr = std::find(values_.begin(), values_.end(),
                         [&](const std::pair<std::string, real_t>& item){
        return item.first == name;
    });

    if(itr == values_.end()){
        throw std::invalid_argument("Invalid variable name");
    }

    return itr->second;
}

template<int dim>
const real_t&
SysState<dim>::operator[](const std::string& name)const{
    auto itr = std::find(values_.begin(), values_.end(),
                         [&](const std::pair<std::string, real_t>& item){
        return item.first == name;
    });

    if(itr == values_.end()){
        throw std::invalid_argument("Invalid variable name");
    }

    return itr->second;
}

template<int dim>
void
SysState<dim>::clear(){

    std::for_each(values_.begin(), values_.end(),
                  [](std::pair<std::string, real_t>& item){
        item.second = 0.0;
    });
}

template<int dim>
std::array<real_t, dim>
SysState<dim>::get_values()const{

    std::array<real_t, dim> copy;
    std::transform(values_.begin(), values_.end(),
                   copy.begin(), [](const std::pair<std::string, real_t>& item){
       return item.second;
    });
}

}

}

#endif // SYSTEM_STATE_H
#ifndef OBSERVER_BASE_H
#define OBSERVER_BASE_H

#include "kernel/base/config.h"


#ifdef USE_LOG
#include "kernel/utilities/logger.h"
#endif

#include <stdexcept>
#include <string>

namespace kernel{

/// \brief Base class for implementing
/// the observer pattern
template<typename ResourceTp>
class ObserverBase
{
public:

    /// \brief The resource type
    typedef ResourceTp resource_t;

    /// \brief Destructor
    virtual ~ObserverBase();

    /// \brief Update. Notify the observer that the
    /// resource is observing has been changed
    virtual void update(const resource_t& resource)=0;

    /// \brief Read the resource
    virtual void read(resource_t& r)const=0;

    /// \brief Returns true if the resouce has been updated
    bool is_updated()const{return updated_;}

    /// \brief Set the update flag
    void set_updated_flag(bool flag){updated_=flag;}

    /// \brief The name of the observer
    std::string_view name()const{return name_;}

    /// \brief Set the name of the observer
    void set_name(std::string&& name){name_ = name;}

protected:

    /// \brief Constructor
    ObserverBase()=default;

    /// \brief Constructor
    ObserverBase(const resource_t& resource);

    /// \brief Constructor
    ObserverBase(std::string&& name);

private:

    /// \brief The resource used
    resource_t resource_;

    /// \brief Flag indicating if the resource has been
    /// updated
    mutable bool updated_;

    /// \brief The name of the observer
    std::string name_;

};


template<typename ResourceTp>
ObserverBase<ResourceTp>::~ObserverBase()
{}

template<typename ResourceTp>
ObserverBase<ResourceTp>::ObserverBase(const typename ObserverBase<ResourceTp>::resource_t& resource)
    :
      resource_(resource),
      updated_(false),
      name_("")
{}

template<typename ResourceTp>
ObserverBase<ResourceTp>::ObserverBase(std::string&& name)
    :
      resource_(),
      updated_(false),
      name_(name)
{}

template<typename ResourceTp>
void
ObserverBase<ResourceTp>::update(const resource_t& resource){

#ifdef USE_LOG
    Logger::log_info("Update observer "+name_);
#endif
    resource_ = resource;
    updated_ = true;
}

template<typename ResourceTp>
void
ObserverBase<ResourceTp>::read(typename ObserverBase<ResourceTp>::resource_t& r)const{

#ifdef USE_LOG
    Logger::log_info("Read observer "+name_);
#endif

    r = resource_;
    updated_ = false;
}


/// \brief Pointer resource
template<typename ResourceTp>
class ObserverBase<ResourceTp*>
{
public:

    /// \brief The resource type
    typedef ResourceTp resource_t;

    /// \brief Destructor
    virtual ~ObserverBase();

    /// \brief Update. Notify the observer that the
    /// resource is observing has been changed
    virtual void update(const resource_t& resource)=0;

    /// \brief Read the resource
    virtual const resource_t& read()const=0;

    /// \brief Returns true if the resouce has been updated
    bool is_updated()const{return updated_;}

    /// \brief Set the update flag
    void set_updated_flag(bool flag){updated_=flag;}

    /// \brief The name of the observer
    std::string_view name()const{return name_;}

    /// \brief Set the name of the observer
    void set_name(std::string&& name){name_ = name;}

protected:

    /// \brief Constructor
    ObserverBase();

    /// \brief Constructor
    ObserverBase(const resource_t& resource);

    /// \brief Constructor
    ObserverBase(std::string&& name);

private:

    /// \brief The resource used
    const resource_t* resource_;

    /// \brief Flag indicating if the resource has been
    /// updated
    mutable bool updated_;

    /// \brief The name of the observer
    std::string name_;

};

template<typename ResourceTp>
ObserverBase<ResourceTp*>::~ObserverBase()
{}

template<typename ResourceTp>
ObserverBase<ResourceTp*>::ObserverBase()
    :
      resource_(nullptr),
      updated_(false)
{}

template<typename ResourceTp>
ObserverBase<ResourceTp*>::ObserverBase(const typename ObserverBase<ResourceTp*>::resource_t& resource)
    :
      resource_(&resource),
      updated_(false)
{}

template<typename ResourceTp>
ObserverBase<ResourceTp*>::ObserverBase(std::string&& name)
    :
      resource_(),
      updated_(false),
      name_(name)
{}

template<typename ResourceTp>
void
ObserverBase<ResourceTp*>::update(const resource_t& resource){

#ifdef USE_LOG
    Logger::log_info("Update observer "+name_);
#endif

    resource_ = &resource;
    updated_ = true;
}

template<typename ResourceTp>
const typename ObserverBase<ResourceTp*>::resource_t&
ObserverBase<ResourceTp*>::read()const{

    if(resource_ == nullptr){
        throw std::logic_error("Null resource ptr. Update resource");
    }

#ifdef USE_LOG
    Logger::log_info("Read observer "+name_);
#endif

    updated_=false;
    return *resource_;
}


}

#endif // OBSERVER_BASE_H

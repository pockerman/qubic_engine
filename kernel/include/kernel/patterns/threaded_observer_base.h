#ifndef THREADED_OBSERVER_BASE_H
#define THREADED_OBSERVER_BASE_H

#include "kernel/patterns/observer_base.h"

namespace kernel{

template<typename MutexTp, typename ResourceTp>
class ThreadedObserverBase: public ObserverBase<ResourceTp>
{
public:

    /// \brief The mutex type
    typedef MutexTp mutex_t;
    typedef typename ObserverBase<ResourceTp>::resource_t resource_t;

    /// \brief Destructor
    virtual ~ThreadedObserverBase();

protected:

    /// \brief Constructor
    ThreadedObserverBase()=default;

    /// \brief Constructor
    ThreadedObserverBase(const resource_t& resource);

    /// \brief The mutex used
    mutable mutex_t mutex_;
};

template<typename MutexTp, typename ResourceTp>
ThreadedObserverBase<MutexTp, ResourceTp>::ThreadedObserverBase(const resource_t& resource)
    :
      ObserverBase<ResourceTp>(resource),
      mutex_()
{}

template<typename MutexTp, typename ResourceTp>
ThreadedObserverBase<MutexTp, ResourceTp>::~ThreadedObserverBase()
{}


/// \brief Pointer resource
template<typename MutexTp, typename ResourceTp>
class ThreadedObserverBase<MutexTp, ResourceTp*>: public ObserverBase<ResourceTp*>
{
public:

    /// \brief The mutex type
    typedef MutexTp mutex_t;
    typedef typename ObserverBase<ResourceTp*>::resource_t resource_t;

    /// \brief Destructor
    virtual ~ThreadedObserverBase();

protected:

    /// \brief Constructor
    ThreadedObserverBase()=default;

    /// \brief Constructor
    ThreadedObserverBase(const resource_t& resource);

    /// \brief The mutex used
    mutable mutex_t mutex_;
};

template<typename MutexTp, typename ResourceTp>
ThreadedObserverBase<MutexTp, ResourceTp*>::ThreadedObserverBase(const resource_t& resource)
    :
      ObserverBase<ResourceTp*>(resource),
      mutex_()
{}

template<typename MutexTp, typename ResourceTp>
ThreadedObserverBase<MutexTp, ResourceTp*>::~ThreadedObserverBase()
{}

}

#endif // THREADED_OBSERVER_BASE_H

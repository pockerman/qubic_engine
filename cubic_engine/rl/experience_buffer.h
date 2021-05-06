#ifndef EXPERIENCE_BUFFER_H
#define EXPERIENCE_BUFFER_H

#include "cubic_engine/base/cubic_engine_types.h"

#include "boost/noncopyable.hpp"

#include <deque>
#include <memory>

namespace cengine{
namespace rl {

///
/// \brief The ExperienceBuffer class. A buffer based on
/// std::deque to accumulate items of type ExperienceTp.
///
template<typename ExperienceTp, class AllocatorTp = std::allocator<ExperienceTp>>
class ExperienceBuffer: private boost::noncopyable{

public:

    typedef ExperienceTp value_t ;
    typedef ExperienceTp experience_t;
    typedef AllocatorTp allocator_t ;


    ///
    /// \brief ExperienceBuffer
    ///
    explicit ExperienceBuffer(uint_t capacity);

    ///
    /// \brief append Add the experience item in the buffer
    ///
    void append(const experience_t& experience);

    ///
    /// \brief size
    /// \return
    ///
    uint_t size()const noexcept{return static_cast<uint_t>(experience_.size());}

    ///
    /// \brief empty. Returns true if the buffer is empty
    /// \return
    ///
    bool empty()const noexcept{return experience_.empty();}

    ///
    /// \brief sample. Sample batch_size experiences from the
    /// buffer and transfer them in the BatchTp container.
    ///
    template<typename BatchTp>
    void sample(uint_t batch_size, BatchTp& batch);


private:

    ///
    /// \brief experience_. Buffer for the experience
    ///
    std::deque<experience_t, allocator_t> experience_;

    ///
    /// \brief max_size_ Max size of the buffer
    ///
    uint_t max_size_;

};

template<typename ExperienceTp, class AllocatorTp>
ExperienceBuffer<ExperienceTp, AllocatorTp>::ExperienceBuffer(uint_t max_size)
    :
      experience_(),
      max_size_(max_size)
{}

template<typename ExperienceTp, class AllocatorTp>
void
ExperienceBuffer<ExperienceTp, AllocatorTp>::append(const experience_t& experience){

}

template<typename ExperienceTp, class AllocatorTp>
template<typename BatchTp>
void
ExperienceBuffer<ExperienceTp, AllocatorTp>::sample(uint_t batch_size, BatchTp& batch){

}


}
}

#endif // EXPERIENCE_BUFFER_H

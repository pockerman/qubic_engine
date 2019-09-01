#ifndef LIKELIHOOD_MODEL_BASE_H
#define LIKELIHOOD_MODEL_BASE_H

namespace cengine
{

template<typename RsltTp>
class LikelihoodModelBase
{
public:

    typedef RsltTp result_type;

    LikelihoodModelBase() = default;
    virtual ~LikelihoodModelBase() = default;

    virtual result_type operator()()const=0;

};


}

#endif // LIKELIHOOD_MODEL_BASE_H

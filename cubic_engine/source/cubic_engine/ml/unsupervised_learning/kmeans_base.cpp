#include "parml/unsupervised_learning/kmeans_base.h"



namespace parml
{
    
template<typename DataPoint>
KMeansBase<DataPoint>::KMeansBase(const KMeansControl& cntrl)
:
cntrl_(cntrl),
info_(),
clusters_()
{}

namespace kmeans_detail
{
#ifdef PARML_DEBUG

    void assert_comon(const std::string& func_msg,size_type nrows,size_type k,size_type max_itrs){
        
        using utilities::ExeLogicError;
        std::string msg = func_msg + "DataSet is empty";
        Assert(nrows!= 0,ExeLogicError(msg));
        msg = func_msg + " Invalid number of neighbors k cannot be zero";
        Assert(k!=0,ExeLogicError(msg));
        msg = func_msg + " Invalid number of neighbors. k: "+std::to_string(k)+" not in [1,"+std::to_string(nrows)+")";
        Assert(k < nrows,ExeLogicError(msg));
        msg = func_msg+" Invalid number of iterations. Max iterations cannot be 0";
        Assert(max_itrs != 0, ExeLogicError(msg));
    }
#endif
}

template class KMeansBase<ArmaVecDataSet<real_type> >;
template class KMeansBase<ArmaVecDataSet<size_type> >;
template class KMeansBase<ArmaVecDataSet<int> >;
template class KMeansBase<ArmaVecDataSet<float> >;
    
}

/* 
 * File:   knn_detail.h
 * Author: david
 *
 * Created on October 18, 2016, 10:03 AM
 */

#ifndef KNN_DETAIL_H
#define	KNN_DETAIL_H

#include <map>

namespace parml
{
    
    namespace knn_detail
    {
        
        size_type compute_majority_vote(const std::map<size_type,size_type>& majority_vote);
        
    }
}



#endif	/* KNN_DETAIL_H */


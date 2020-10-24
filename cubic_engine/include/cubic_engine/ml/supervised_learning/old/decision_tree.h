/* 
 * File:   decision_tree.h
 * Author: david
 *
 * Created on December 3, 2015, 10:30 PM
 */

#ifndef DECISION_TREE_H
#define	DECISION_TREE_H
#include "mllib/base/mllib.h"

#include "algebralib/vectors/Vector.h"
#include "algebralib/matrices/dense_matrix.h"

namespace mllib{

/**
 * A class that represents a decision tree
 */   
template<typename TreeType>
class decision_tree{
  
public:
    
    /**
     * the general template for training the decision tree
     * @param features
     * @param labels
     */
    template<typename Trainer>
    void train(algebralib::DenseMatrix<real_type>& features,
               algebralib::Vector<real_type>& labels,
               const Trainer& trainer);
    
    void predict();
    
private:
    
    /**
     * the object that holds the representation
     * of the tree data structure 
     */
    TreeType tree_data_;
    
    
};


template<typename TreeType>
template<typename Trainer>
void 
decision_tree<TreeType>::train(algebralib::DenseMatrix<real_type>& features,
                               algebralib::Vector<real_type>& labels,
                               const Trainer& trainer){
    
    
    
    //trainer.train(tree_data_features,)
}
    
    
}



#endif	/* DECISION_TREE_H */


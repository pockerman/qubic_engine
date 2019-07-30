/* 
 * File:   linear_logistic_regression.h
 * Author: david
 *
 * Created on January 27, 2016, 10:58 PM
 */

#ifndef LINEAR_LOGISTIC_REGRESSION_H
#define	LINEAR_LOGISTIC_REGRESSION_H

#include "mllib/base/mllib.h"

#include "algebralib/vectors/Vector.h"
#include "algebralib/matrices/dense_matrix.h"

namespace mllib
{

    
//forward declarations
    template<typename T> class  LinearFunc;
    template<typename T> class  CutOffFunc;
/**
 * class for linear logistic regression classification
 * learners
 */
    template<typename CFn,typename Phi>
class LinearLogisticRegression
{
    
    
public:
    
    /**
     * ctor initialize by passing the linear function
     * used to model the hypothesis to approximare
     */
    LinearLogisticRegression(LinearFunc<real_type>& func);
    
    
    /**
    * train the model. Data entries should be
    * of numerical type. Trainer is the type of the object
    * that actually trains the learner.
    * Solver is allowed to change the features and labels
    */
    template<typename Trainer,typename PartFn>
    void train(algebralib::DenseMatrix<real_type>& features,
               algebralib::Vector<real_type>& labels,
               Trainer& solver,PartFn& partitioner);
    
    /**
      * predict 
      */
     template<typename V>
     void predict(V& in,V& out){}
            
private:
    
    
     /**
       * pointer to the hypothesis model
       * the learner is using. Non const as the
       * object is allowed to alter this 
       */
    LinearFunc<real_type>* hfn_;
    
    /**
      * pointer to the cost function object
      * we want to minimize
      */
    CFn cfn_;
    
    /**
     * the cut of function to use for mapping back
     * into the [0,1] interval
     */
    Phi phi_;
    
};

template<typename Trainer,typename Phi,typename PartFn>
void 
LinearLogisticRegression::train(algebralib::DenseMatrix<real_type>& features,
                                algebralib::Vector<real_type>& labels,
                                Trainer& trainer,Phi& phi,PartFn& partitioner){
    
    
    trainer.solve(features,labels,*hfn_,cfn_,phi_,partitioner);
    
    
}


    
    
    
}//mllib



#endif	/* LINEAR_LOGISTIC_REGRESSION_H */


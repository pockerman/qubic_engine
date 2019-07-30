/* 
 * File:   linear_regression.h
 * Author: david
 *
 * Created on October 31, 2015, 8:14 PM
 */

#ifndef LINEAR_REGRESSION_H
#define	LINEAR_REGRESSION_H

#include "mllib/base/mllib.h"

//parframepp
#include "parframepp/base/parframe_common.h"

#include "mllib/functions/linear_function.h"
#include "algebralib/vectors/Vector.h"
#include "algebralib/matrices/dense_matrix.h"


namespace mllib{
   
    
/**
 * linear regression class. The hypothesis
 * and the cost functions are passed as template parameters
 * HFn is assumed to be a linear function
 */
 template<typename CFn>
 class LinearRegression
 {
     
 public:
     
     
     /**
      * constructor create an object by passing the
      * hypothesis and the cost functions 
      */
     LinearRegression(LinearFunc<real_type>& hfn,CFn& cfn)
            :
            hfn_(&hfn),
            cfn_(cfn)
     {}
     
     
   /**
    * train the model. Data entries should be
    * of numerical type. Solver is the type of the object
    * that actually solves the minimization problem.
    * Solver is allowed to change the features and labels
    */
    template<typename Solver,typename PartFn>
    void train(algebralib::DenseMatrix<real_type>& features,
               algebralib::Vector<real_type>& labels,
               Solver& solver,PartFn& partitioner);
     
     
     /**
      * predict 
      */
     template<typename V>
     void predict(V& in,V& out){}
     
     
     private:
         
         /**
          * pointer to the hypothesis model
          * the regressor is using
          */
         LinearFunc<real_type>* hfn_;
         
         /**
          * pointer to the cost function object
          * we want to minimize
          */
         CFn* cfn_;
     
 };
 
 
 //the general template for training
 template<typename CFn>
 template<typename Solver,typename PartFn>
 void
 LinearRegression<CFn>::train(algebralib::DenseMatrix<real_type>& features,
                                  algebralib::Vector<real_type>& labels,
                                  Solver& solver,PartFn& partitioner){
     
     
     solver.solve(features,labels,*hfn_,*cfn_,partitioner);
 }
    
    
}//mllib



#endif	/* LINEAR_REGRESSION_H */


/* 
 * File:   squared_loss_func.h
 * Author: david
 *
 * Created on November 29, 2016, 6:11 PM
 */

#ifndef SQUARED_LOSS_FUNC_H
#define	SQUARED_LOSS_FUNC_H

#include "parml/base/parml.h"
#include "parml/functions/distance_types.h"

namespace parml
{
  
//forward declarations
template<typename LabelType> class SquaredLossFunc;
template<typename T> class ArmaVecDataSet;
    
/**
 * @brief Class for calculating losses of the form
 * 
 * $$ (y-\hat(y))^2 $$
 * 
 * This is the general template for scalar targets
 */

template<typename LabelType> 
class SquaredLossFunc
{
    
public:
    
    /**
     * @brief The type of the lable
     */
    typedef LabelType label_type;
    
    /**
     * @brief The return type
     */
    typedef real_type return_type;
    
    /**
     * @brief Constructor
     */
    SquaredLossFunc();
    
    /**
     * @brief Return the squared difference between the target label and the predicted target
     */
    real_type evaluate(const LabelType& target,const LabelType& prediction)const;
      
};

//template and inline methods
template<typename LabelType>
real_type
SquaredLossFunc<LabelType>::evaluate(const LabelType& target, const LabelType& prediction) const{
    
    real_type diff = (target - prediction);
    return diff*diff;
}

template<typename LabelType>
using ScalarSquaredLoss = SquaredLossFunc<LabelType>;

//forward declarations
template<typename VecType> class VecMetricFuncBase;

/**
 * @brief Class for calculating losses of the form
 * 
 * $$ (y-\hat(y))^2 $$
 * 
 * This is specialized template for  ArmaVecDataSet targets
 */
template<typename T>
class SquaredLossFunc<ArmaVecDataSet<T> >
{
    
    public:
        
    /**
      * @brief The type of the lable
      */
     typedef ArmaVecDataSet<T> label_type;

     /**
      * @brief The return type
      */
     typedef real_type return_type;

     /**
      * @brief Constructor 
      */
     SquaredLossFunc(const VecMetricFuncBase<ArmaVecDataSet<T> >& metric);
     

     /**
      * @brief Return the squared difference between the target label and the predicted target
      */
     real_type evaluate(const ArmaVecDataSet<T>& target,const ArmaVecDataSet<T>& prediction)const; 
    
    private:
        
        /**
         * @brief Pointer to the metric that performs the calculation
         */
        const VecMetricFuncBase<ArmaVecDataSet<T> >* metric_;
};

template<typename T>
using VectorSquaredLoss = SquaredLossFunc<ArmaVecDataSet<T> >;




    

}



#endif	/* SQUARED_LOSS_FUNC_H */


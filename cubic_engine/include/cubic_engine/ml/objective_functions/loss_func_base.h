/* 
 * File:   abstract_loss.h
 * Author: david
 *
 * Created on November 29, 2016, 6:04 PM
 */

#ifndef LOSS_FUNC_BASE_H
#define	LOSS_FUNC_BASE_H

namespace parml
{
    
/**
 * 
 * \par
 * In statistics and machine learning, a loss function encodes
 * the severity of getting a label wrong. This is am important
 * special case of a cost function (see AbstractCost), where
 * the cost is computed as the average loss over a set, also
 * known as (empirical) risk.
 *
 * \par
 * It is generally agreed that loss values are non-negative,
 * and that the loss of correct prediction is zero. This rule
 * is not formally checked, but instead left to the various sub-classes.
 */
class LossFuncBase
{
    
    
};    
}



#endif	/* ABSTRACT_LOSS_H */


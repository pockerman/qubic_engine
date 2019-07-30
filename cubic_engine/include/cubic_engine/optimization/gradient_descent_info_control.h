/* 
 * File:   gradient_descent_info.h
 * Author: david
 *
 * Created on September 30, 2016, 6:25 PM
 */

#ifndef GRADIENT_DESCENT_INFO_CONTROL_H
#define	GRADIENT_DESCENT_INFO_CONTROL_H

#include "parml/algorithms/gradient_descent_type.h"
#include "parml/base/algorithm_control.h"
#include "parml/base/algorithm_info.h"
#include "utilities/scalar_value_traits.h"

namespace parml
{
    
/**
 * @brief A small struct that holds data
 * about the Gradient descent algorithm
 */    
struct GDControl: public AlgControl{
    
    /**
     * @brief The learning rate
     */ 
    real_type learning_rate;
    
    /**
     * @brief The number of training examples
     */
    size_type n_training_examples;
    
    
    /**
     * @brief Number of features
     */
    size_type n_features;
    
    /**
     * @brief Constructor. Construct by passing the number of iterations, the learning
     * rate and optionally the tolerance
     */
    GDControl(size_type itrs,real_type rate,real_type tol=ParML::tolerance())
            :
            AlgControl(itrs,tol),
            learning_rate(rate),
            n_training_examples(ParML::iuint()),
            n_features(ParML::iuint())
    {}
    
     /**
     * @brief print the control data to the given stream
     */
    virtual std::ostream& print(std::ostream& out)const;
};

inline
std::ostream& 
operator<<(std::ostream& out, const GDControl& c){
    return c.print(out);
}


/**
 * @brief The object to return when the gradient descent algorithm finishes
 */
struct GDInfo: public AlgInfo{
    
    
    /**
     * @brief The learning rate
     */ 
    real_type learning_rate;
    
    /**
     * @brief The type of gradient descent
     */
    gradient_descent_type::Type type;
    
    /**
     * @brief constructor
     */
    GDInfo()
    :
    AlgInfo(),
    learning_rate(utilities::scalar_value_traits<real_type>::zero()),
    type(gradient_descent_type::Type::INVALID_TYPE)
    {} 
    
    /**
     * @brief Print the information related to the application
     * of gradient descent algorithm to the given stream.
     */
    virtual std::ostream& print(std::ostream& out)const;
};

inline
std::ostream&
operator<<(std::ostream& out,const GDInfo& info){
    
    return info.print(out);  
}
   
}//parml



#endif	/* GRADIENT_DESCENT_INFO_H */


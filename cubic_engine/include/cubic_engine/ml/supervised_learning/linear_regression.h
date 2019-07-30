/* 
 * File:   linear_regression.h
 * Author: david
 *
 * Created on June 25, 2016, 12:54 PM
 */

#ifndef LINEAR_REGRESSION_H
#define	LINEAR_REGRESSION_H

#include "parml/base/parml.h"
#include "parml/base/parml_runtime.h"
#include "parml/base/data_adaptor.h"
#include "parml/base/parml_config.h"
#include "parml/functions/linear_function.h"
#include "parml/supervised_learning/supervised_learner_base.h"

namespace parml
{
    
 //forward declarations
 template<typename DataInput> class LinearRegression;
    
template<typename DataInput>
class LinearRegression: public supervised_learner_base<DataInput,LinearFunc>
{
    
    public:
    
            /**
              *@brief The type of the object that holds the data matrix and the labels
              */
            typedef typename supervised_learner_base<DataInput,LinearFunc>::data_input_type data_input_type;

            /**
              * @brief the output of the classification
              */
            typedef ClassifierOutput<real_type>::output_type output_type;

            /**
              * @brief Constructor
              */
            LinearRegression(data_input_type datain)
                  :
            supervised_learner_base<DataInput,LinearFunc>(datain)
            {}

            /**
             * @brief Set the number of parameters for the model 
             */
            void set_model_parameters(size_type nparams);

            /**
              * @brief Train the linear model
              */
            template<typename Trainer,typename ErrorFuncType>
            typename Trainer::output_type
            train(const Trainer& trainer,const ErrorFuncType& func);


            /**
              * @brief Predict the class for the given data point
              */
            template<typename DataPoint>
            output_type predict(const DataPoint& point)const;
};

template<typename DataInput>
template<typename Trainer,typename ErrorFuncType>
typename Trainer::output_type
LinearRegression<DataInput>::train(const Trainer& trainer,const ErrorFuncType& func){
    
    
    return trainer.solve(DataInput::type_1(this->data_in_),
                         DataInput::type_2(this->data_in_),
                         func,this->model_);
    
}

template<typename DataInput>
void 
LinearRegression<DataInput>::set_model_parameters(size_type nparams){
    
    this->model_.set_n_coeffs(nparams);
}

template<typename DataInput>
template<typename DataPoint>
real_type 
LinearRegression<DataInput>::predict(const DataPoint& point)const{
    
    return this->model_.value(point);
}

    
    
}//parml



#endif	/* LINEAR_REGRESSION_H */


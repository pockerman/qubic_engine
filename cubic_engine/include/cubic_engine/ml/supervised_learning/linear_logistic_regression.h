/* 
 * File:   linear_logistic_regression.h
 * Author: david
 *
 * Created on June 20, 2016, 11:56 AM
 */

#ifndef LINEAR_LOGISTIC_REGRESSION_H
#define	LINEAR_LOGISTIC_REGRESSION_H

#include "parml/base/parml.h"
#include "parml/base/parml_runtime.h"
#include "parml/base/data_adaptor.h"
#include "parml/base/parml_config.h"
#include "parml/functions/linear_transformed_function.h"
#include "parml/supervised_learning/supervised_learner_base.h"


namespace parml
{
    
    /**
     * @brief Serial logistic classification algorithm
     */
    template<typename DataInput,typename Transformer>
    class LinearLogisticClassifier:public supervised_learner_base<DataInput,LinearFuncTransformed<Transformer> >
    {
        
        public:
            
            /**
              *@brief The type of the object that holds the data matrix and the labels
              */
            typedef typename supervised_learner_base<DataInput,LinearFuncTransformed<Transformer> >::data_input_type data_input_type;

            /**
              * @brief the output of the classification
              */
            typedef ClassifierOutput<real_type>::output_type output_type;
            
            /**
             * @brief Constructor
             */
            LinearLogisticClassifier(data_input_type datain);
           
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
 
 template<typename DataInput,typename Transformer>
 LinearLogisticClassifier<DataInput,Transformer>::LinearLogisticClassifier(typename LinearLogisticClassifier<DataInput,Transformer>::data_input_type datain)
:
supervised_learner_base<DataInput,LinearFuncTransformed<Transformer> >(datain)
{
                this->model_.set_weights_grad_calculation_flag(true);
}

template<typename DataInput,typename Transformer>
template<typename Trainer,typename ErrorFuncType>
typename Trainer::output_type
LinearLogisticClassifier<DataInput,Transformer>::train(const Trainer& trainer,const ErrorFuncType& func){
    
    
    return trainer.solve(DataInput::type_1(this->data_in_),
                         DataInput::type_2(this->data_in_),func,this->model_);
    
}

template<typename DataInput,typename Transformer>
void 
LinearLogisticClassifier<DataInput,Transformer>::set_model_parameters(size_type nparams){
    
    this->model_.set_n_coeffs(nparams);
}

template<typename DataInput,typename Transformer>
template<typename DataPoint>
typename ClassifierOutput<real_type>::output_type
LinearLogisticClassifier<DataInput,Transformer>::predict(const DataPoint& point)const{
    
    return this->model_.value(point);         
}
    
    
    
}//parml


#endif	/* LINEAR_LOGISTIC_REGRESSION_H */


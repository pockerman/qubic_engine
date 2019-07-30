/* 
 * File:   svm.h
 * Author: david
 *
 * Created on July 7, 2016, 2:27 PM
 */

#ifndef SVM_H
#define	SVM_H

#include "parml/base/parml.h"
#include "parml/base/parml_runtime.h"
#include "parml/supervised_learning/supervised_learner_base.h"

namespace parml
{
    
/**
 * @brief Implementation of support vector machines
 */

template<typename DataInput,typename DummyType>
class Svm: public supervised_learner_base<DataInput,DummyType>
{
    
    public:
        
        /**
          *@brief The type of the object that holds the data matrix and the labels
          */
         typedef typename supervised_learner_base<DataInput,DummyType>::data_input_type data_input_type;

        /**
          * @brief the output of the classification
          */
        typedef ClassifierOutput<real_type>::output_type output_type;
        
        
        /**
          * @brief Constructor
           */
        Svm(data_input_type datain)
                  :
            supervised_learner_base<DataInput,DummyType>(datain)
        {}

       

        /**
          * @brief Train the linear model
          */
        template<typename Trainer,typename KernelType>
        typename Trainer::output_type
        train(Trainer& trainer,const KernelType& func);


        /**
          * @brief Predict the class for the given data point
          */
        template<typename DataPoint>
        output_type predict(const DataPoint& point)const;
        
};

template<typename DataInput,typename DummyType>
template<typename Trainer,typename KernelType>
typename Trainer::output_type
Svm<DataInput,DummyType>::train(Trainer& trainer,const KernelType& func){
    
    
    return trainer.solve(DataInput::type_1(this->data_in_),
                         DataInput::type_2(this->data_in_),func);
}


    
}//parml



#endif	/* SVM_H */


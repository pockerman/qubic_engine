/* 
 * File:   classifier_base.h
 * Author: david
 *
 * Created on June 20, 2016, 12:04 PM
 */

#ifndef SUPERVISED_LEARNER_BASE_H
#define	SUPERVISED_LEARNER_BASE_H

namespace parml
{
    
/**
 * @brief Base class for all classifiers in parML.
 * Classification is a supervised learning algorithm. These
 * algorithms require labels to perform the training. This
 * class is agnostic of how training is performed.
 */    
template<typename DataInput,typename Model>
class supervised_learner_base
{
    
    public:
        
        /**
          *@brief The type of the object that holds the data matrix and the labels
          */
       typedef  const DataInput*    data_input_type;
       
       /**
        * @brief Expose the type of the matrix
        */
       typedef typename DataInput::type1 matrix_input_type;
       
       /**
        * @brief Expose the type of the container that holds the labels
        */
       typedef typename DataInput::type2 label_input_type;
        
    protected:
        
        /**
         * @brief Constructor
         */
        supervised_learner_base(data_input_type data);
        
        
    public:
        
        
       /**
        * @brief Destructor
        */
       virtual ~supervised_learner_base()
       {}
       
       
       /**
        * @brief Return the model
        */
       Model& get_model(){return model_;}
       
       
       /**
        * @brief Return the model
        */
       const Model& get_model()const{return model_;}
       
    protected:
        
        /**
         * @brief Pointer to the object that holds the
         * data points and the labels
         */
        data_input_type data_in_;
        
        
        /**
         * @brief The model the learner is using
         */
        Model model_;       
};

template<typename DataInput,typename Model>
supervised_learner_base<DataInput,Model>::supervised_learner_base(const DataInput* data)
:
data_in_(data),
model_()
{}   
}



#endif	/* CLASSIFIER_BASE_H */


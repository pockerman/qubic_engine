/* 
 * File:   naive_bayes.h
 * Author: david
 *
 * Created on May 7, 2016, 3:30 PM
 */

#ifndef NAIVE_BAYES_H
#define	NAIVE_BAYES_H

#include "parml/base/parml.h"
#include "utilities/default_value.h"

#include <boost/optional.hpp>

namespace parml
{
    
/**
 * classification using Naive Bayes method.
 * The runner template parameter is the object that
 * actually does the classification. We have flavors
 * working serially, using multithreading and MPI
 */
template<typename Runner>
class NaiveBayes
{
    
    public:
        
        /**
         * Expose the data input
         */
        
        typedef typename Runner::data_input_type data_input_type;
        
        
        /**
          * the type of the probability generator
          */
        typedef typename Runner::probability_generator_type probability_generator_type;
        
        
        /**
          * the type of the smoother to use
          */
        typedef typename Runner::smoother_type smoother_type;
        
        /**
         * expose the type of the output after classification. Typically
         * this will be an integer representing the class label
         */
        typedef typename Runner::output_type output_type;
        
        NaiveBayes(data_input_type data = utilities::default_value<data_input_type>::value)
                :
                runner_(data)
        {}
        
        
        /**
         * read/write access to the data used for training
         */
        data_input_type get_data(){return runner_.get_data();}
        
        /**
         * read/write access to the runner object
         */
        Runner& get_runner(){return runner_;}
        
        
        /**
         * train the model. Use the data set passed upon creation to
         * train the model
         *
         */
        void train();
        
        /**
         * classify the given data point
         * @param data_point the data point to classify
         * @return the class index or whatever else Runner::output_type signifies
         */
        template<typename DPointType>
        output_type classify(const DPointType& data_point);
        
        /**
         * get the class probability for the class index
         * @return 
         */
        boost::optional<real_type> get_class_probability(size_type cls)const;
        
        
        /**
         * get the number of training examples for the given class index
         * that contain the feature with index featureidx with value val
         * @param data_point
         * @param cls
         * @return 
         */
        size_type get_class_n_training_examples_with_feature_val(size_type cls,
                                                                 size_type featureidx,
                                                                 size_type val)const;
        
    protected:
        
        /**
         * members that are either exposed through the
         * public API via write references or pointers
         * are always under protected
         */
        
        Runner runner_;    
};

template<typename Runner>
void
NaiveBayes<Runner>::train(){
    
    runner_.train();
}

template<typename Runner>
template<typename DPointType>
typename Runner::output_type
NaiveBayes<Runner>::classify(const DPointType& data_point){
    
    return runner_.predict(data_point);
}

template<typename Runner>
boost::optional<real_type> 
NaiveBayes<Runner>::get_class_probability(size_type cls)const{
    
    return runner_.get_class_probability(cls);
}

template<typename Runner>
size_type 
NaiveBayes<Runner>::get_class_n_training_examples_with_feature_val(size_type cls,
                                                                 size_type featureidx,
                                                                 size_type val)const{
    
    
    return runner_.get_class_n_training_examples_with_feature_val(cls,featureidx,val);
}



}//parml



#endif	/* NAIVE_BAYES_H */


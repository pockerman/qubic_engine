/* 
 * File:   naive_bayes_impl.h
 * Author: david
 *
 * Created on May 7, 2016, 4:41 PM
 */

#ifndef NAIVE_BAYES_IMPL_H
#define	NAIVE_BAYES_IMPL_H

#include "parml/base/parml.h"
#include "utilities/scalar_value_traits.h"
#include "utilities/default_value.h"
#include "utilities/map_utilities.h"

#ifdef PARML_DEBUG
#include "utilities/exceptions.h"
#endif

#include <map>

#include <boost/optional.hpp>

namespace parml
{
    
    namespace kmeans_detail{
        
        template<typename MapType,typename DataInput>
        void compute_class_occurence(MapType& class_occurence,
                                     const DataInput& y){
            
            typedef typename MapType::iterator iterator;
            
            for(size_type i=0; i<y.rows(); ++i){
        
                size_type cls = y.operator[](i);

                utilities::add_or_update_map(class_occurence,cls,
                                         utilities::scalar_value_traits<size_type>::one(),
                                         [](iterator itr){itr->second += 1;});   
            }   
        } 
    }
    
template<typename DataInput,typename ProbGenerator,typename Smoother>
class naive_bayes_base
{
    
    public:
        
        /**
          * this runner does not modify the data given to it
          */
         typedef  DataInput* data_input_type;

         typedef typename DataInput::type1 type1;
         typedef typename DataInput::type2 type2;
         
         /**
          * the type of the probability generator
          */
         typedef ProbGenerator probability_generator_type;
         
         /**
          * the type of the smoother to use
          */
         typedef Smoother smoother_type;
         
         /**
          * ctor create by passing the data the algorithm should work on
          * @param data
          */
         naive_bayes_base(data_input_type data = utilities::default_value<data_input_type>::value)
                 :
                 data_in_(data),
                 prob_generator_(),
                 smoother_()
         {}
         
         /**
          * read/write access to the probability generator
          * @return the probability generator
          */
         ProbGenerator& get_probability_generator(){return prob_generator_;} 
         
         /**
          * read/write access to the smoother
          */
         Smoother& get_smoother(){return smoother_;}
         
    protected:
         
         data_input_type data_in_;
         
         /**
          * the probability generator
          */
         ProbGenerator prob_generator_;
         
         /**
          * the object used for smoothing
          */
         Smoother smoother_;
        
};


template<typename DataInput,typename ProbGenerator,typename Smoother>
class naive_bayes_serial: public naive_bayes_base<DataInput,ProbGenerator,Smoother>
{
    
    public:
        
        typedef typename naive_bayes_base<DataInput,ProbGenerator,Smoother>::data_input_type data_input_type; 
        typedef typename naive_bayes_base<DataInput,ProbGenerator,Smoother>::probability_generator_type probability_generator_type;
        typedef typename naive_bayes_base<DataInput,ProbGenerator,Smoother>::smoother_type smoother_type;
        
        /**
         * expose the type of the output after classification. Typically
         * this will be an integer representing the class label
         */
        typedef std::pair<size_type,real_type> output_type;
        
        naive_bayes_serial(data_input_type datain = utilities::default_value<data_input_type>::value)
                :
                naive_bayes_base<DataInput,ProbGenerator,Smoother>(datain)
        {}
        
        void train();
        
        /**
         * return true if and only if the given class index is in the
         * class indices
         */
        bool data_set_has_class(size_type classidx)const;
        
        /**
         * get the class probability for the class index
         * @return 
         */
        boost::optional<real_type> get_class_probability(size_type cls)const;
        
        /**
         * get the number of training examples in the given class
         * @param cls the class index
         * @return the number of training examples in the given class
         */
        size_type get_class_n_training_examples(size_type cls)const;
        
        
        /**
         * get the number of training examples for the given class index
         * that contain the feature with index featureidx with value val
         * @param data_point
         * @param cls
         * @return 
         */
        size_type get_class_n_training_examples_with_feature_val(size_type cls,size_type featureidx,size_type val)const;
        
        /**
         * get the probability that the given data point belongs to the
         * given class.
         * @return boost::optional that holds the calculate probability
         */
        template<typename DPointType>
        boost::optional<real_type> get_data_point_class_probability(const DPointType& data_point,size_type cls)const;
        
        
        /**
         * classify the given data point
         * @param data_point the data point to classify
         * @return the class index or whatever else Runner::output_type signifies
         */
        template<typename DPointType>
        output_type predict(const DPointType& data_point)const;
        
    private:
        
        /**
         * a map that keeps the number of classes in the dataset
         * and how many times this class occurs
         */
        std::map<size_type,size_type> class_occurence_;
        
        /**
         * the number of training examples in each category
         */
        std::map<size_type,size_type> train_to_class_map_;
        
        
};

template<typename DataInput,typename ProbGenerator,typename Smoother>
bool 
naive_bayes_serial<DataInput,ProbGenerator,Smoother>::data_set_has_class(size_type classidx)const{
    
    typedef std::map<size_type,size_type>::const_iterator const_iterator;
    
    const_iterator it = class_occurence_.find(classidx);
    
    if(it != class_occurence_.end()){
        
        return true;
    }
    
    return false;
    
}

template<typename DataInput,typename ProbGenerator,typename Smoother>
size_type 
naive_bayes_serial<DataInput,ProbGenerator,Smoother>::get_class_n_training_examples_with_feature_val(size_type cls,
                                                                                                    size_type featureidx,
                                                                                                    size_type val)const{
    
    if(!data_set_has_class(cls))
        return utilities::scalar_value_traits<size_type>::zero();
    
    //loop over rows of the data matrix
    auto X = DataInput::type_1(this->data_in_);
    auto y = DataInput::type_2(this->data_in_);
    
    size_type rows = X->rows();
    size_type cols = X->cols();
    
#ifdef PARML_DEBUG
    //attempting to search for a feature
    //that does not exist is an error
    
    using utilities::ExeInvalidIndex;
    const std::string msg="Index: "+std::to_string(featureidx)+" out of bounds: [0, "+std::to_string(cols)+")";
    Assert(featureidx<cols,ExeInvalidIndex(msg));
#endif
    
    size_type counter = utilities::scalar_value_traits<size_type>::zero();
    for(size_type r=0; r<rows; ++r){
        
        for(size_type c=0; c<cols; ++c){
            
            if(c==featureidx && (*y)[r] == cls){
                
                if((*X)(r,c) == val)
                    counter++;    
            }
        }
        
    }
        
   return counter; 

}

template<typename DataInput,typename ProbGenerator,typename Smoother>
size_type 
naive_bayes_serial<DataInput,ProbGenerator,Smoother>::get_class_n_training_examples(size_type cls)const{
   
    
    typedef std::map<size_type,size_type>::const_iterator const_iterator;
    
    const_iterator it = class_occurence_.find(cls);
    
    if(it != class_occurence_.end()){
        
        size_type counter = it->second;
        return counter;
    }
    
    return utilities::scalar_value_traits<size_type>::zero();
}

template<typename DataInput,typename ProbGenerator,typename Smoother>
boost::optional<real_type> 
naive_bayes_serial<DataInput,ProbGenerator,Smoother>::get_class_probability(size_type cls)const{
    
    typedef std::map<size_type,size_type>::const_iterator const_iterator;
    
    const_iterator it = class_occurence_.find(cls);
    
    if(it != class_occurence_.end()){
        
        size_type counter = it->second;
        
        //how many examples
        auto y = DataInput::type_2(this->data_in_);
        size_type nexamples = y->rows();
        
        return counter/static_cast<real_type>(nexamples);
    }
    
    return boost::none;
}

template<typename DataInput,typename ProbGenerator,typename Smoother>
template<typename DPointType>
boost::optional<real_type> 
naive_bayes_serial<DataInput,ProbGenerator,Smoother>::get_data_point_class_probability(const DPointType& data_point,
                                                                                       size_type cls)const{
    
  //loop over the features of test example
  //find the probability that the feature has for the given class
  real_type total_prob = utilities::scalar_value_traits<real_type>::one();
  
  auto X = DataInput::type_1(this->data_in_);
  
  for(size_type f=0; f<data_point.rows(); ++f){
      
      
      
      //get the feature value
      size_type feature_val = data_point[f];
      
      std::cout<<"For feature: "<<f<<" of data point...with feature value: "<<feature_val<<std::endl;
      
      //how many times the feature with the given value 
      //and the given class appears in the data set
      size_type appearance_times = get_class_n_training_examples_with_feature_val(cls,f,feature_val);
      
      std::cout<<"\t ...appears in class "<<cls<<" "<<appearance_times<<" times..."<<std::endl;
      
      //ask the smoother to calculate the probability
      real_type prob = this->smoother_(appearance_times,X->rows());
      
      std::cout<<"\t ...smoothed probability "<<prob<<std::endl;
      
      //ask the prob generator to calculate a probability
      //value based on prob and the feature value
      real_type prob2 = this->prob_generator_(prob,feature_val);
      
      std::cout<<"\t ...binomial probability "<<prob2<<std::endl;
      
      //std::cout<<"For feature: "<<f<<" probability2 "<<prob2<<std::endl;
      
      total_prob *= prob2;   
  }
  
  
  //std::cout<<"\t....For class: "<<cls<<" probability: "<<*get_class_probability(cls)<<std::endl;
  
  total_prob *= *get_class_probability(cls);
  
  std::cout<<"\t....Total probability: "<<total_prob<<std::endl;
    
  return total_prob;    
}


template<typename DataInput,typename ProbGenerator,typename Smoother>
void
naive_bayes_serial<DataInput,ProbGenerator,Smoother>::train(){
    
#ifdef PARML_DEBUG
    using utilities::ExeNullPointer;
    Assert(this->data_in_ !=nullptr,ExeNullPointer());
#endif
    
    auto y = DataInput::type_2(this->data_in_);
    kmeans_detail::compute_class_occurence(class_occurence_,*y);
    
}

template<typename DataInput,typename ProbGenerator,typename Smoother>
template<typename DPointType>
typename naive_bayes_serial<DataInput,ProbGenerator,Smoother>::output_type 
naive_bayes_serial<DataInput,ProbGenerator,Smoother>::predict(const DPointType& data_point)const{
    
    
    //we loop over the classes and attempt to
    //calculate the probability that the data point 
    //belongs to a given class the class with the maximum probability
    //along with the probability value is returned
    
    real_type max_prob = utilities::scalar_value_traits<real_type>::zero();
    size_type pclassidx = ParML::iuint();
    typedef std::map<size_type,size_type>::const_iterator class_iterator;
    
    class_iterator classb = class_occurence_.begin();
    class_iterator classe = class_occurence_.end();
    
    while(classb != classe){
        
        size_type classidx = classb->first;
        std::cout<<"Class index: "<<classidx<<std::endl;
        
        boost::optional<real_type> prob = get_data_point_class_probability(data_point,classidx);
        
        //std::cout<<"Max probability is "<<max_prob<<" pcclassidx "<<pclassidx<<std::endl;
        //std::cout<<"probability is "<<*prob<<std::endl;
        if((*prob - max_prob) > ParML::tolerance()){
            
            max_prob = *prob;
            pclassidx = classidx;
        }
        
        classb++;   
    }
    
  return std::make_pair(pclassidx,max_prob);  
}


}



#endif	/* NAIVE_BAYES_IMPL_H */


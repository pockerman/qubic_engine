/* 
 * File:   neuron.h
 * Author: david
 *
 * Created on November 26, 2016, 6:07 PM
 */

#ifndef NEURON_H
#define	NEURON_H

namespace parml
{
    
/**
 * @brief The geneal template class  that represents
 * a neuron in an artificial neural network
 */
template<typename ActiveFunc>
class NNNeuron
{
    
    public:
        
        /**
         * @brief Expose the type of the activation function
         */
        typedef ActiveFunc act_func_type;
};
}



#endif	/* NEURON_H */


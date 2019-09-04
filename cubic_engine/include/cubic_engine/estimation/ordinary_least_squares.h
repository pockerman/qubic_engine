#ifndef ORDINARY_LEAST_SQUARES_H
#define ORDINARY_LEAST_SQUARES_H

#include "cubic_engine/base/cubic_engine_types.h"
#include <boost/noncopyable.hpp>

namespace cengine
{
	
class OLSMatrixDescriptor
{
public:

	typedef DynMat<real_t> matrix_type;
	
	/// \brief Constructor
	OLSMatrixDescriptor();
	
	/// \brief Set the coefficients matrix
	void set_matrix(const std::string& /*name*/, matrix_type& mat){mat_ = &mat;}
	
	/// \brief Returns read/write reference to the coefficient matrix
	matrix_type& get_matrix(const std::string& /*name*/){return mat_;}
	
	/// \brief Returns read reference to the coefficient matrix
	const matrix_type& get_matrix(const std::string& /*name*/)const{return mat_;}
	
protected:

	/// \brief The coefficients matrix for OLS
	matrix_type* mat_;
	
};
	
/**
 * Implementation of Ordinary Least Squares Estimator
 * 
 */  
	
template<typename StateType, typename InputType, typename MatrixDescriptorType>
class OLS: private boost::noncopyable
{
	
public:

	   typedef typename MatrixDescriptorType::matrix_type matrix_type;
	   typedef StateType state_type;
	   typedef InputType input_type;
	
	   /// \brief Constructor	
	   explicit OLS(state_type& state);
	   
	   /// \brief Estimate
	   void estimate(const input_type& z);
	   
	   /// \brief Set the coefficients matrix
       void set_matrix(const std::string& name, matrix_type& mat){matrix_descriptor_.set_matrix(name, mat);}
	   
private:

	/// \brief Reference to the type the estimator is
	/// trying to estimate (fit)
	state_type& state_;
	
	/// \brief The object that describes the matrix structure 
	/// of the OLS problem
	MatrixDescriptorType matrix_descriptor_;
	
};

template<typename StateType, typename InputType, typename MatrixDescriptorType>
OLS<StateType, InputType, MatrixDescriptorType>::OLS(state_type& state)
:
state_(state),
matrix_descriptor_()
{}

template<typename StateType, typename InputType, typename MatrixDescriptorType>
void 
OLS<StateType, InputType, MatrixDescriptorType>::estimate(const typename OLS<StateType, InputType, MatrixDescriptorType>::input_type& z){
	
	const auto& H = matrix_descriptor_.get_matrix("H");
	auto H_T = trans(H);
	auto H_T_H = H_T*H;
	
	// update the state
	state_ = inv(H_T_H)*H_T*z;	
}

	
}

#endif

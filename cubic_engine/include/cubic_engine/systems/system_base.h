/* 
 * File:   system_base.h
 * Author: david
 *
 * Created on November 9, 2016, 9:09 AM
 */

#ifndef SYSTEM_BASE_H
#define	SYSTEM_BASE_H

#include "robosim/base/robosim.h"
#include <armadillo>

#include <initializer_list>
#include<boost/noncopyable.hpp>

namespace robosim
{
    
    
/**
 * @brief Base class for representing stochastic systems. Concrete classes
 * of this class are LinearSteadySys, LinearUnsteadySys, NonLinearSys.
 * 
 * $$\mathbf{x}_t = f(\mathbf{x}_{t-1},\mathbf{u}_{t-1},\mathbf{w}_{t-1})$$
 * $$\mathbf{z}_t = h(\mathbf{x}_t,\mathbf{v}_t)$$
 */
    
class SystemBase: private boost::noncopyable
{
public:
    
    
    
    
    /**
     * @brief Expose the type of the matrix the system uses
     */
    typedef arma::Mat<real_type> mat_type;
    
    /**
     * @brief Expose the type of the vector the system uses. This
     * is a column vector
     */
    typedef arma::vec vec_type;
    
    /**
     * @brief Constructor
     */
    SystemBase();
    
    
    /**
     * @brief Destructor 
     */
    virtual ~SystemBase();
    
    /**
     * @brief Return a reference to the state transition matrix
     */
    mat_type& get_A_mat(){return At_;}
    
    /**
     * @brief Return a reference to the state transition matrix
     */
    const mat_type& get_A_mat()const{return At_;}
    
    
    
    /**
     * @brief Return a reference to the state covariance matrix 
     */
    mat_type& get_P_mat(){return Pt_;}
    
    /**
     * @brief Return a reference to the state covariance matrix 
     */
    const mat_type& get_P_mat()const{return Pt_;}
    
    /**
     * @brief Set the P matrix 
     */
    void set_P_matrix(const std::initializer_list<real_type>& list){Pt_ = list;}
    
    /**
     * @brief Return the process noise covariance matrix Q.
     */
    mat_type& get_Q_mat(){return Qt_;}
    
    /**
     * @brief Return the process noise covariance matrix Q.
     */
    const mat_type& get_Q_mat()const{return Qt_;}
    
    
    /**
     * @brief Return the  
     */
    mat_type& get_W_mat(){return Wt_;}
    
    /**
     * @brief Return the  
     */
    const mat_type& get_W_mat()const{return Wt_;}
    
    
    /**
     * @brief Return a reference to the measurement noise covariance matrix
     */
    mat_type& get_R_mat(){return Rt_;}
    
    /**
     * @brief Return a reference to the measurement noise covariance matrix
     */
    const mat_type& get_R_mat()const{return Rt_;}
    
    
    /**
     * @brief Return a reference to the measurement noise covariance matrix
     */
    mat_type& get_H_mat(){return Ht_;}
    
    /**
     * @brief Return a reference to the measurement noise covariance matrix
     */
    const mat_type& get_H_mat()const{return Ht_;}
    
    
    
    /**
     * @brief Return a reference to the current control vector
     */
    vec_type& get_u_vec(){return ut_;}
    
    /**
     * @brief Return a reference to the current control vector
     */
    const vec_type& get_u_vec()const{return ut_;}
    
    /**
     * @brief Return a reference to the current measurement vector
     */
    vec_type& get_z_vec(){return zt_;}
    
    /**
     * @brief Return a reference to the current measurement vector
     */
    const vec_type& get_z_vec()const{return zt_;}
    
     /**
     * @brief Return a reference to the current measurement vector
     */
    vec_type& get_x_vec(){return xt_;}
    
    /**
     * @brief Return a reference to the current measurement vector
     */
    const vec_type& get_x_vec()const{return xt_;}
    
    /**
     *@brief Set the x vector
     */
    void set_x_vector(const std::initializer_list<real_type>& list){xt_ = list;}
    
    
    /**
     * @brief Initialize the underlying data structure.
     * @param n The dimension of the state vector
     * @param m The dimension of the input vector
     * @param k The dimension of the measurement vector
     */
    virtual void initialize(size_type n,size_type m,size_type p,size_type l);
    
    
protected:
    
    /**
     * @brief The state vector
     */
    vec_type xt_;
    
    /**
     * @brief The control vector
     */
    vec_type ut_;
    
    /**
     * @brief The measurement vector
     */
    vec_type zt_;
    
    /**
     * @brief The error vector associated with the state vector
     */
    vec_type wt_;
    
    /**
     * @brief The error vector associated with the  measurement
     */
    vec_type vt_;
    
    /**
      * @brief The state transition matrix. This a square  matrix of \f$n\times n\f$
      * dimension where \f$n\f$ is the dimension of the state vector. This is actually
      * a Jacobian matrix of partial derivatives defined as
      * \f$ A_{i,j} = \frac{\partial f_i}{\partial x_j}\f$
      */
    mat_type At_;
    
    /**
     * @brief This is an \f$ n \times nw \f$ Jacobian matrix of partial
     * derivatives defined as
     * 
     * $$W_{[i,j]} = \frac{\partial f_{[i]}}{\partial w_{[j]}}$$
     */
    mat_type Wt_;
    
    /**
     * @brief The process noise covariance matrix
     */
    mat_type Qt_;
    
    /**
     * @brief The coefficients matrix for the measurement equation
     */
    mat_type Ht_;
    
    /**
     * @brief
     */
    mat_type Vt_;
    
    /**
     * @brief The measurement error covariance matrix
     */
    mat_type Rt_;
    
    /**
     * @brief The error covariance matrix associated with the state vector
     */
    mat_type Pt_;
        
};    
    
}



#endif	/* SYSTEM_BASE_H */


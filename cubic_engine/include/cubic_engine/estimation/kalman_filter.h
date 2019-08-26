/**
 * Linear Kalman Filter implementation. See
 * An Introduction to the Kalman Filter, TR 95-041
 * by
 * Greg Welch1 and Gary Bishop
 */

#include "cubic_engine/base/cubic_engine_types.h"
#include <boost/noncopyable.hpp>

#include <map>
#include <string>
#include <stdexcept> //for std::invalid_argument


namespace cengine
{
	
class KalmanFilterMatrixDescriptor
{
	
public:

	typedef DynMat<real_t> matrix_type;


	/// \brief Constructor
	KalmanFilterMatrixDescriptor();

	
	/// \brief Destructor
	virtual KalmanFilterMatrixDescriptor(){}


	/// \brief Return the names of the matrics involved
	/// in the definition of the Kalman Filter 
	const std::vector<std::string> get_names()const;

	
	/// \brief Set the matrix with the given name
	void set_matrix(const std::string& name, matrix_type& matrix);
	
	
    /// \brief get_matrix. Returns read reference to the matrix_name matrix
    const matrix_type* get_matrix(const std::string& matrix_name)const{return mat_descrp_.get_matrix(matrix_name);}
    
  
    /// \brief get_matrix. Returns read reference to the matrix_name matrix
    matrix_type* get_matrix(const std::string& matrix_name){return mat_descrp_.get_matrix(matrix_name);}

	
	/// \brief Update the matrix descriptors
	virtual void update_matrices(){}
	
protected:


	/// \brief Pointers to the matrices describing the system
    std::map<std::string, matrix_type* > matrix_maps_;
	
};

/// \detailed Implements the Linear Kalman Filter algorithm. The following algorithm is
/// implemented
/// x_k = A_kx_{k-1} + B_k u_k + w_{k-1}
/// y_k = H_k x_k + v_k
///
/// where w_k and v_k  represent process and measurement noise respectively. They are assumed
/// independent and normally distributed:
///
/// p(w) ~ N(0,Q)
/// p(v) ~ N(0,R)
///
/// The following matrices dimensions are assumed:
///
/// A n x n
/// B n x l
/// u l x 1
/// H m x n
/// y m x 1
/// x n x 1
///
/// The Kalman Filter algorithm is a predictor-corrector process:
///
/// Prediction step
/// ---------------
///
/// X_{k}^{-} = A_{k-1}X_{k-1} + B_k U_k
/// P_{k}^{-} = A_{k-1}P_{k-1}A_{k-1}^T + Q_{k-1} // Process covariance prediction
///
/// Update/correct step
/// ---------------
///
/// K_k = P_{k}^{-}H_{k}^T(H_kP_k{-}H_{k}^T + R_k)^{-1}// Gain matrix it says how much the predictions should be corrected
/// \hat{X}_k = X_{k}^{-} + K_k(y_k - H_kX_{k}^{-})
/// \hat{P}_k = (I - K_kH_k)P_{k}^{-}
///
/// where
///
/// K n x m
///
/// As the class manipulates a a given state vector it does not make  sense to
/// be copyable
template<typename StateTp, typename InputTp, typename MatrixDescriptorTp>
class KalmanFilter: private boost::noncopyable
{
public:

	typedef StateTp state_type;
	typedef InputTp input_type;
	typedef typename input_type::control_type control_type;
	typedef typename input_type::measurement_type measurement_type; 
    typedef MatrixDescriptorTp matrix_description_type;
    typedef typename  matrix_description_type::matrix_type matrix_type;

    /// \brief Constructor
    explicit KalmanFilter(state_type* x=nullptr);
    
    
    /// \brief Destructor
    virtual ~KalmanFilter();
    
    /// \brief Predicts the state and the process covariance matrix using
    /// X_{k}^{-} = A_{k-1}X_{k-1} + B_k U_k
    /// P_{k}^{-} = A_{k-1}P_{k-1}A_{k-1}^T + Q_{k-1}
    /// u: The control input
    virtual void predict(const control_type& u);
    
    
    /// \brief Updates the state and covariance matrices using the measurement
    ///K_k = P_{k}^{-}H_{k}^T(H_kP_k{-}H_{k}^T + R_k)^{-1}
    ///\hat{X}_k = X_{k}^{-} + K_k(y_k - H_kX_{k}^{-})
    ///\hat{P}_k = (I - K_kH_k)P_{k}^{-}
    virtual void update(const measurement_type& z);
    
    
    /// \brief Perform one iteration i.e. predict + update of the Kalman filter
    void iterate(const input_type& input);
    
    
    /// \brief Returns the matrices names
    std::vector<std::string> get_matrix_names()const{return matrix_descriptor_.get_names();}
    
    
    /// \brief Set the pointer matrix
    void set_matrix(const std::string& name, matrix_type& mat){matrix_descriptor_.set_matrix(name, mat);}
    
    
    /// \brief Set the pointer to the state vector the filter manipulates
    void set_state_vector(state_type& x ){x_ = &x;}

protected:

    /// \brief The object that describes the matrices
    matrix_description_type matrix_descriptor_;
    
    /// The Kalman gain calculated during the course of the update step
    matrix_type K_;
    
    /// \brief The state vector that the filter manipulates
    state_type* x_;
};

template<typename StateTp, typename InputTp, typename MatrixDescriptorTp>
KalmanFilter<StateTp, InputTp, MatrixDescriptorTp>::KalmanFilter(typename KalmanFilter<StateTp, InputTp, MatrixDescriptorTp>::state_type* x)
        :
        matrix_descriptor_(),
         K_(),
         x_(x)
{}

template<typename StateTp, typename InputTp, typename MatrixDescriptorTp>
KalmanFilter<StateTp, InputTp, MatrixDescriptorTp>::~KalmanFilter()
{}


template<typename StateTp, typename InputTp, typename MatrixDescriptorTp>
void
KalmanFilter<StateTp, InputTp, MatrixDescriptorTp>::predict(const typename KalmanFilter<StateTp, InputTp, MatrixDescriptorTp>::control_type& u){

    if(!x_){
        throw std::runtime_error("state vector has not been assigned");
    }

    auto& A = *matrix_descriptor_.get_matrix("A");
    auto& B = *matrix_descriptor_.get_matrix("B");

    //predict the state vector
    *x_ = A*(*x_) + B*u;

    //predict the covariance matrix
    auto& P = *matrix_descriptor_.get_matrix("P");
    auto& Q = *matrix_descriptor_.get_matrix("Q");
    auto A_T = trans( A );

    P = (A*(P*A_T)) + Q;

}


void
KalmanFilter<StateTp, InputTp, MatrixDescriptorTp>::update(const const typename KalmanFilter<StateTp, InputTp, MatrixDescriptorTp>::measurement_type& z){

    auto& H = *matrix_descriptor_.get_matrix("H");
    auto& P = *matrix_descriptor_.get_matrix("P");
    auto& R = *matrix_descriptor_.get_matrix("R");
    auto H_T = trans(H);

    auto S = H*P*H_T + R;
    auto S_inv = inv(S);

    // compute gain matrix
    K_ = P*H_T*S_inv;

    auto& x = *x_;
    auto innovation = z - H*x;

    x += K*innovation;

    IdentityMatrix<real_t> I(K.rows());

    // update covariance matrix
    P = (I - K*H)*P;
}


template<typename StateTp, typename InputTp, typename MatrixDescriptorTp>
void 
KalmanFilter<StateTp, InputTp, MatrixDescriptorTp>::iterate(const InputTp& input){

    predict(input.get_u());
    update(input.get_z());
    matrix_descriptor_.update();
}


    
}

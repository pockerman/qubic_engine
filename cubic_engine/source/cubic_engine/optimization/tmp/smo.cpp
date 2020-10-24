#include "parml/algorithms/smo.h"

namespace parml
{
    namespace smo_detail
    {
 
        void 
        clip(real_type& a, real_type H, real_type L){
            
            if(a > H)
                a = H;
            else if(a < L)
                a = L;
        }
        
        
        void get_non_zero_indices(const std::map<size_type,real_type>& error_cache,
                                  std::vector<size_type>& indices,real_type tol){
            
            indices.clear();
            
            typedef std::map<size_type,real_type>::const_iterator const_iterator;
            
            const_iterator errb = error_cache.begin();
            const_iterator erre = error_cache.end();
            
            while(errb != erre){
                
                if(errb->second > tol){
                    
                    indices.push_back(errb->first);
                }
                
                errb++;
            }
        }
        
        
        void determine_H_and_L(real_type& H,real_type& L,
                               real_type C,real_type ai,real_type aj,int yi,int yj){
            
            const real_type zero = utilities::scalar_value_traits<real_type>::zero();
            
            if (yi != yj){
                
                L = std::max(zero, aj - ai);
                H = std::min(C, C + aj - ai);
            }
            else{
                
                L = std::max(zero, aj + ai - C);
                H = std::min(C, aj + ai);
            }    
        }
        
        void determine_beta_coeff(real_type& b,
                                  real_type C, real_type ai,real_type aj,
                                  real_type b1,real_type b2){
            
            if( (0 < ai) && (C > ai)){
                b = b1;
            }
            else if( (0 < aj) && (C > aj) ){
                b = b2;
            }
            else{
                b = 0.5*(b1 + b2);
            }
            
        }
        
    }//smo_detail
    
    
    std::ostream& 
    SmoControl::print(std::ostream& out)const{ 
        
        this->AlgControl::print(out);
        out<<"C: "<<C<<std::endl;
        out<<"Small value: "<<small_value<<std::endl;
        out<<"Number of examples: "<<n_training_examples<<std::endl;
        out<<"Number of features: "<<n_features<<std::endl;
        return out;
    }
    
    
    SmoBase::SmoBase(SmoControl& data)
    :
    data_(data),
    alphas_(),
    b_(utilities::scalar_value_traits<real_type>::zero()),
    error_cache_()
    {}
    
   
    
    std::ostream& 
    SmoBase::print_alphas(std::ostream& out)const{
        
       typedef std::vector<real_type>::const_iterator const_iterator; 
       const_iterator alphasb = alphas_.begin();
       const_iterator alphase = alphas_.end(); 
       
       size_type counter = 0;
       
       while(alphasb != alphase ){
           
           out<<"training point: "<<counter++<<" "<<*alphasb<<std::endl;
           alphasb++;
       }
        
       return out;    
    }

     Smo<serial>::Smo(SmoControl& data)
            :
            SmoBase(data)
    {}
     
    //multicore implementation
    Smo<cput_c11>::Smo(SmoControl& data)
            :
            SmoBase(data),
            par_indexed_object<cput_c11>()
    {}
    
    
#ifdef PARML_HAS_MPI
    
    Smo<mpi_par>::Smo(SmoControl& data)
            :
            SmoBase(data),
            par_indexed_object<mpi_par>()
    {}
    
    
    size_type 
    Smo<mpi_par>::get_non_zero_indices_on_master(std::vector<size_type>& indices,size_type i,real_type Ei){
        
        //update the error cache
        utilities::add_or_update_map(this->SmoBase::error_cache_,i,Ei);
    
        //get the non zero indices
        smo_detail::get_non_zero_indices(this->SmoBase::error_cache_,indices,
                                         this->SmoBase::data_.small_value);
        
        
    }
    
#endif 
}

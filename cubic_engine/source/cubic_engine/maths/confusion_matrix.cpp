#include "cubic_engine/maths/confusion_matrix.h"
#include <exception>
namespace cengine
{


ConfusionMatrix::ConfusionMatrix(const std::map<uint_t, std::vector<uint_t>>& data)
    :
      matrix_()
{
    build_from(data);
}


void
ConfusionMatrix::build_from(const std::map<uint_t, std::vector<uint_t>>& data){

    if(data.empty()){
        throw std::invalid_argument("Cannot construct ConfusionMatrix from empty data");
    }

    uint_t rows = data.size();
    uint_t cols = data.begin()->second.size();

    if(rows != cols){
        throw std::invalid_argument("A ConfusionMatrix is a square matrix but rows are: "+
                                    std::to_string(rows)+
                                    " and columns: "+
                                    std::to_string(cols));
    }

     matrix_.resize(rows, cols);
     auto itr = data.begin();

     for(uint_t r=0; r<rows; ++r){

         const auto& row = itr->second; //data[r];
         for(uint_t c=0; c<cols; ++c){

            matrix_(r, c) = row[c];
         }

         ++itr;
     }
}


uint_t
ConfusionMatrix::true_positives()const{
    return 0;
}

real_t
ConfusionMatrix::recall()const{

}


real_t
ConfusionMatrix::f1_score()const{

}

}

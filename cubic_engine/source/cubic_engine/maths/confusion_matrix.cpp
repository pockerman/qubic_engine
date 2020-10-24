#include "cubic_engine/maths/confusion_matrix.h"
#include <exception>

namespace cengine
{

real_t
ConfusionMatrix::accuracy()const{

    return true_positives()/total_count();
}

real_t
ConfusionMatrix::misclassification_rate()const{
    return 1.0 - accuracy();
}

uint_t
ConfusionMatrix::true_positives()const{

    uint_t result = 0;
    for(uint_t r=0; r<matrix_.rows(); ++r){

        result += get_class_counts(r);
    }
    return result;
}

real_t
ConfusionMatrix::recall_class(uint_t c)const{

    // how many times did we predicted the class
    uint_t class_counts = get_class_counts(c);

    // how many times we missed it overall
    uint class_incorrect_counts = get_class_incorrect_counts(c);

    return class_counts/ (class_incorrect_counts + class_counts);
}

uint_t
ConfusionMatrix::get_class_counts(uint_t c)const{

    if( c >= matrix_.rows()){
        throw std::invalid_argument("Invlaid class index. Index "+
                                    std::to_string(c)+
                                    " not in [0, "+
                                    std::to_string(matrix_.rows())+
                                    ")");
    }

    return matrix_(c ,c);
}

uint_t
ConfusionMatrix::get_class_counts_as_other_class(u_int c, uint_t other)const{

    if( c >= matrix_.rows()){
        throw std::invalid_argument("Invlaid class index. Index "+
                                    std::to_string(c)+
                                    " not in [0, "+
                                    std::to_string(matrix_.rows())+
                                    ")");
    }

    if( other >= matrix_.rows()){
        throw std::invalid_argument("Invlaid class index. Index "+
                                    std::to_string(other)+
                                    " not in [0, "+
                                    std::to_string(matrix_.rows())+
                                    ")");
    }

    return matrix_(c, other);

}

uint_t
ConfusionMatrix::get_class_incorrect_counts(uint_t c)const{

    if( c >= matrix_.rows()){
        throw std::invalid_argument("Invlaid class index. Index "+
                                    std::to_string(c)+
                                    " not in [0, "+
                                    std::to_string(matrix_.rows())+
                                    ")");
    }

    uint_t result = 0;

    for(uint_t r = 0; r<matrix_.rows(); ++r){

        if( r != c){

            result +=  matrix_(c , r);
        }
    }

    return result;

}

std::ostream&
ConfusionMatrix::print(std::ostream& out)const{

    for(uint_t r = 0; r<matrix_.rows(); ++r){

        for(uint_t c = 0; c<matrix_.rows(); ++c){

            out << matrix_(r, c);

            if(c != matrix_.rows() -1){
                out<<" , ";
            }
            else{
                out<<"\n";
            }
        }
        out<<"\n";
    }

    return out;
}
}

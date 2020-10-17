#ifndef COMMON_UITLS_H
#define COMMON_UITLS_H

namespace kernel {
namespace utils {

///
/// \brief Returns the sign of the expression
///
template<typename T>
int sign(const T& expr){

    if(expr < 0){
        return -1;
    }

    return 1;
}

template<typename T>
int sign(const T& expr1, const T& expr2){
    return sign(expr1*expr2);
}

///
/// \brief Square the given expression
///
template<typename T>
T sqr(const T& expr){
    return expr*expr;
}

template<typename T>
void shift(T& a, T& b, T& c, T& d){
    a=b;
    b=c;
    c=d;
}

template<typename T>
void shift(T& a, T& b, T& c){
    a=b;
    b=c;
}

}

}

#endif // COMMON_UITLS_H

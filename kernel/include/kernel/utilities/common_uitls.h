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

///
/// \brief Square the given expression
///
template<typename T>
T sqr(const T& expr){
    return expr*expr;
}

}

}

#endif // COMMON_UITLS_H

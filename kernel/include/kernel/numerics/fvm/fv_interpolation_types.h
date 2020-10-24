#ifndef FV_INTERPOLATION_TYPES_H
#define FV_INTERPOLATION_TYPES_H
namespace kernel {
namespace numerics {

enum class FVInterpolationType{UD, CD,
                               MIX_UD_CD,
                               LINEAR,
                               INVALID_INTERPOLATION_SCHEME};

}

}

#endif // FV_INTERPOLATION_TYPES_H

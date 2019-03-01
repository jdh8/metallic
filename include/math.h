#ifndef _MATH_H
#define _MATH_H

#include "bits/specifiers.h"

#if __STDC_VERSION__ >= 199901L || __cplusplus >= 201103L
#define FLT_EVAL_METHOD __FLT_EVAL_METHOD__

#if FLT_EVAL_METHOD == 0
typedef float float_t;
typedef double double_t;
#elif FLT_EVAL_METHOD == 1
typedef double float_t;
typedef double double_t;
#elif FLT_EVAL_METHOD == 2
typedef long double float_t;
typedef long double double_t;
#else
#error Unknown FLT_EVAL_METHOD
#endif

#define NAN      __builtin_nanf("")
#define INFINITY __builtin_inff()
#define HUGE_VALF __builtin_huge_valf()
#define HUGE_VAL  __builtin_huge_val()
#define HUGE_VALL __builtin_huge_vall()
#define FP_ILOGBNAN __INT_MAX__
#define FP_ILOGB0   (-__INT_MAX__ - (-1 == ~0))  // INT_MIN

#define FP_NAN       0
#define FP_INFINITE  1
#define FP_ZERO      2
#define FP_SUBNORMAL 3
#define FP_NORMAL    4

#define MATH_ERRNO     1
#define MATH_ERREXCEPT 2

#if defined(__wasm__) || defined(__FAST_MATH__)
#define math_errhandling 0
#else
#define math_errhandling MATH_ERREXCEPT
#endif
#endif /* C99 or C++11 */

#if __STDC_VERSION__ >= 201112L
#define _TGMATH_REAL(x, function) _Generic((x) * 1ULL, \
    unsigned long long: function,                      \
    float: function##f,                                \
    double: function,                                  \
    long double: function##l                           \
)
#else
#define _TGMATH_REAL(x, function) __builtin_choose_expr(                                             \
    __builtin_types_compatible_p(__typeof__((x) * 1ULL), float), function##f, __builtin_choose_expr( \
    __builtin_types_compatible_p(__typeof__((x) * 1ULL), long double), function##l, function         \
))
#endif

#if __STDC_VERSION__ >= 199901L
#define fpclassify(x) __builtin_fpclassify(FP_NAN, FP_INFINITE, FP_NORMAL, FP_SUBNORMAL, FP_ZERO, x)
#define isfinite(x)   __builtin_isfinite(x)
#define isinf(x)      __builtin_isinf(x)
#define isnan(x)      __builtin_isnan(x)
#define isnormal(x)   __builtin_isnormal(x)
#define signbit(x)    _TGMATH_REAL(x, __builtin_signbit)(x)
#define isgreater(x, y)      __builtin_isgreater(x, y)
#define isgreaterequal(x, y) __builtin_isgreaterequal(x, y)
#define isless(x, y)         __builtin_isless(x, y)
#define islessequal(x, y)    __builtin_islessequal(x, y)
#define islessgreater(x, y)  __builtin_islessgreater(x, y)
#define isunordered(x, y)    __builtin_isunordered(x, y)
#endif /* C99 */

#ifdef __cplusplus
extern "C" {
#endif

#define _Scalar double
#define _SUFFIX
#include "bits/mathcalls.h"
#undef _Scalar
#undef _SUFFIX

#if __STDC_VERSION__ >= 199901L || defined(__cplusplus)
#define _Scalar float
#define _SUFFIX f
#include "bits/mathcalls.h"
#undef _Scalar
#undef _SUFFIX

#define _Scalar long double
#define _SUFFIX l
#include "bits/mathcalls.h"
#undef _Scalar
#undef _SUFFIX
#endif

#ifdef __cplusplus
} // extern "C"
#endif

#ifdef _XOPEN_SOURCE
#define M_E        2.71828182845904523536
#define M_LOG2E    1.44269504088896340736
#define M_LOG10E   0.43429448190325182765
#define M_LN2      0.69314718055994530942
#define M_LN10     2.30258509299404568402
#define M_PI       3.14159265358979323846
#define M_PI_2     1.57079632679489661923
#define M_PI_4     0.78539816339744830962
#define M_1_PI     0.31830988618379067154
#define M_2_PI     0.63661977236758134308
#define M_2_SQRTPI 1.12837916709551257390
#define M_SQRT2    1.41421356237309504880
#define M_SQRT1_2  0.70710678118654752440
#endif

#endif /* math.h */

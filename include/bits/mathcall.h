/* This file is part of Metallic, a C++ library for WebAssembly.
 *
 * Copyright (C) 2017 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#define _CAT2(a, b) a##b
#define _CAT(a, b) _CAT2(a, b)

#ifdef _SUFFIX
#define _MATHCALL(f) _CAT(f, _SUFFIX)
#else
#define _MATHCALL(f) f
#endif

_Scalar _MATHCALL(fabs)(_Scalar);
_Scalar _MATHCALL(fmod)(_Scalar, _Scalar);

_Scalar _MATHCALL(exp)(_Scalar);
_Scalar _MATHCALL(log)(_Scalar);
_Scalar _MATHCALL(log10)(_Scalar);

_Scalar _MATHCALL(pow)(_Scalar, _Scalar);
_Scalar _MATHCALL(sqrt)(_Scalar);

_Scalar _MATHCALL(sin)(_Scalar);
_Scalar _MATHCALL(cos)(_Scalar);
_Scalar _MATHCALL(tan)(_Scalar);
_Scalar _MATHCALL(asin)(_Scalar);
_Scalar _MATHCALL(acos)(_Scalar);
_Scalar _MATHCALL(atan)(_Scalar);
_Scalar _MATHCALL(atan2)(_Scalar, _Scalar);

_Scalar _MATHCALL(sinh)(_Scalar);
_Scalar _MATHCALL(cosh)(_Scalar);
_Scalar _MATHCALL(tanh)(_Scalar);

_Scalar _MATHCALL(ceil)(_Scalar);
_Scalar _MATHCALL(floor)(_Scalar);

_Scalar _MATHCALL(frexp)(_Scalar, int*);
_Scalar _MATHCALL(ldexp)(_Scalar, int);
_Scalar _MATHCALL(modf)(_Scalar, _Scalar*);

_Scalar _MATHCALL(fabs)(_Scalar);
_Scalar _MATHCALL(fmod)(_Scalar, _Scalar);

#if defined(__cplusplus) && defined(_SUFFIX)

extern "C++" {

inline _Scalar abs(_Scalar __x) { return _MATHCALL(fabs)(__x); }
inline _Scalar fabs(_Scalar __x) { return _MATHCALL(fabs)(__x); }
inline _Scalar fmod(_Scalar __x, _Scalar __y) { return _MATHCALL(fmod)(__x, __y); }

inline _Scalar exp(_Scalar __x) { return _MATHCALL(exp)(__x); }
inline _Scalar log(_Scalar __x) { return _MATHCALL(log)(__x); }
inline _Scalar log10(_Scalar __x) { return _MATHCALL(log10)(__x); }

inline _Scalar pow(_Scalar __x, _Scalar __y) { return _MATHCALL(pow)(__x, __y); }
inline _Scalar sqrt(_Scalar __x) { return _MATHCALL(sqrt)(__x); }

inline _Scalar sin(_Scalar __x) { return _MATHCALL(sin)(__x); }
inline _Scalar cos(_Scalar __x) { return _MATHCALL(cos)(__x); }
inline _Scalar tan(_Scalar __x) { return _MATHCALL(tan)(__x); }
inline _Scalar asin(_Scalar __x) { return _MATHCALL(asin)(__x); }
inline _Scalar acos(_Scalar __x) { return _MATHCALL(acos)(__x); }
inline _Scalar atan(_Scalar __x) { return _MATHCALL(atan)(__x); }
inline _Scalar atan2(_Scalar __y, _Scalar __x) { return _MATHCALL(atan2)(__y, __x); }

inline _Scalar sinh(_Scalar __x) { return _MATHCALL(sinh)(__x); }
inline _Scalar cosh(_Scalar __x) { return _MATHCALL(cosh)(__x); }
inline _Scalar tanh(_Scalar __x) { return _MATHCALL(tanh)(__x); }

inline _Scalar ceil(_Scalar __x) { return _MATHCALL(ceil)(__x); }
inline _Scalar floor(_Scalar __x) { return _MATHCALL(floor)(__x); }

inline _Scalar frexp(_Scalar __x, int* __exp) { return _MATHCALL(frexp)(__x, __exp); }
inline _Scalar ldexp(_Scalar __x, int __exp) { return _MATHCALL(ldexp)(__x, __exp); }
inline _Scalar modf(_Scalar __x, _Scalar* __i) { return _MATHCALL(modf)(__x, __i); }

} // extern "C++"

#endif /* C++ and _SUFFIX */

#if (__STDC_VERSION__ >= 199901 || __cplusplus >= 201103)

_Scalar _MATHCALL(remainder)(_Scalar, _Scalar);
_Scalar _MATHCALL(remquo)(_Scalar, _Scalar, int*);
_Scalar _MATHCALL(fma)(_Scalar, _Scalar, _Scalar);
_Scalar _MATHCALL(fmax)(_Scalar, _Scalar);
_Scalar _MATHCALL(fmin)(_Scalar, _Scalar);
_Scalar _MATHCALL(fdim)(_Scalar, _Scalar);
_Scalar _MATHCALL(nan)(const char*);

_Scalar _MATHCALL(exp2)(_Scalar);
_Scalar _MATHCALL(expm1)(_Scalar);
_Scalar _MATHCALL(log2)(_Scalar);
_Scalar _MATHCALL(log1p)(_Scalar);

_Scalar _MATHCALL(cbrt)(_Scalar);
_Scalar _MATHCALL(hypot)(_Scalar, _Scalar);

_Scalar _MATHCALL(asinh)(_Scalar);
_Scalar _MATHCALL(acosh)(_Scalar);
_Scalar _MATHCALL(atanh)(_Scalar);

_Scalar _MATHCALL(erf)(_Scalar);
_Scalar _MATHCALL(erfc)(_Scalar);
_Scalar _MATHCALL(tgamma)(_Scalar);
_Scalar _MATHCALL(lgamma)(_Scalar);

_Scalar _MATHCALL(trunc)(_Scalar);
_Scalar _MATHCALL(nearbyint)(_Scalar);

_Scalar _MATHCALL(round)(_Scalar);
long _MATHCALL(lround)(_Scalar);
long long _MATHCALL(llround)(_Scalar);

_Scalar _MATHCALL(rint)(_Scalar);
long _MATHCALL(lrint)(_Scalar);
long long _MATHCALL(llrint)(_Scalar);

_Scalar _MATHCALL(scalbn)(_Scalar, int);
_Scalar _MATHCALL(scalbln)(_Scalar, long);
int _MATHCALL(ilogb)(_Scalar);
_Scalar _MATHCALL(logb)(_Scalar);
_Scalar _MATHCALL(nextafter)(_Scalar, _Scalar);
_Scalar _MATHCALL(nextoward)(_Scalar, _Scalar);
_Scalar _MATHCALL(copysign)(_Scalar, _Scalar);

#endif /* C99 or C++11 */

#if (__cplusplus >= 201103) && defined(_SUFFIX)

extern "C++" {

inline _Scalar remainder(_Scalar __x, _Scalar __y) { return _MATHCALL(remainder)(__x, __y); }
inline _Scalar remquo(_Scalar __x, _Scalar __y, int* __q) { return _MATHCALL(remquo)(__x, __y, __q); }
inline _Scalar fma(_Scalar __x, _Scalar __y, _Scalar __a) { return _MATHCALL(fma)(__x, __y, __a); }
inline _Scalar fmax(_Scalar __x, _Scalar __y) { return _MATHCALL(fmax)(__x, __y); }
inline _Scalar fmin(_Scalar __x, _Scalar __y) { return _MATHCALL(fmin)(__x, __y); }
inline _Scalar fdim(_Scalar __x, _Scalar __y) { return _MATHCALL(fdim)(__x, __y); }
inline _Scalar nan(const char* __s) { return _MATHCALL(nan)(__s); }

inline _Scalar exp2(_Scalar __x) { return _MATHCALL(exp2)(__x); }
inline _Scalar expm1(_Scalar __x) { return _MATHCALL(expm1)(__x); }
inline _Scalar log2(_Scalar __x) { return _MATHCALL(log2)(__x); }
inline _Scalar log1p(_Scalar __x) { return _MATHCALL(log1p)(__x); }

inline _Scalar cbrt(_Scalar __x) { return _MATHCALL(cbrt)(__x); }
inline _Scalar hypot(_Scalar __x, _Scalar __y) { return _MATHCALL(hypot)(__x, __y); }

inline _Scalar asinh(_Scalar __x) { return _MATHCALL(asinh)(__x); }
inline _Scalar acosh(_Scalar __x) { return _MATHCALL(acosh)(__x); }
inline _Scalar atanh(_Scalar __x) { return _MATHCALL(atanh)(__x); }

inline _Scalar erf(_Scalar __x) { return _MATHCALL(erf)(__x); }
inline _Scalar erfc(_Scalar __x) { return _MATHCALL(erfc)(__x); }
inline _Scalar tgamma(_Scalar __x) { return _MATHCALL(tgamma)(__x); }
inline _Scalar lgamma(_Scalar __x) { return _MATHCALL(lgamma)(__x); }

inline _Scalar trunc(_Scalar __x) { return _MATHCALL(trunc)(__x); }
inline _Scalar nearbyint(_Scalar __x) { return _MATHCALL(nearbyint)(__x); }

inline _Scalar round(_Scalar __x) { return _MATHCALL(round)(__x); }
inline long lround(_Scalar __x) { return _MATHCALL(lround)(__x); }
inline long long llround(_Scalar __x) { return _MATHCALL(llround)(__x); }

inline _Scalar rint(_Scalar __x) { return _MATHCALL(rint)(__x); }
inline long lrint(_Scalar __x) { return _MATHCALL(lrint)(__x); }
inline long long llrint(_Scalar __x) { return _MATHCALL(llrint)(__x); }

inline _Scalar scalbn(_Scalar __x, int __exp) { return _MATHCALL(scalbn)(__x, __exp); }
inline _Scalar scalbln(_Scalar __x, long __exp) { return _MATHCALL(scalbln)(__x, __exp); }
inline int ilogb(_Scalar __x) { return _MATHCALL(ilogb)(__x); }
inline _Scalar logb(_Scalar __x) { return _MATHCALL(logb)(__x); }
inline _Scalar nextafter(_Scalar __x, _Scalar __y) { return _MATHCALL(nextafter)(__x, __y); }
inline _Scalar nextoward(_Scalar __x, _Scalar __y) { return _MATHCALL(nexttoward)(__x, __y); }
inline _Scalar copysign(_Scalar __x, _Scalar __y) { return _MATHCALL(copysign)(__x, __y); }

} // extern "C++"

#endif /* C++11 and _SUFFIX */

#if (__cplusplus >= 201103)

extern "C++" {

inline bool isfinite(_Scalar __x)
{
#ifdef __GNUC__
    return __builtin_isfinite(__x);
#else
    return fabs(__x) < 1 / _Scalar();
#endif
}

inline bool isinf(_Scalar __x)
{
#ifdef __GNUC__
    return __builtin_isinf(__x);
#else
    return fabs(__x) >= 1 / _Scalar();
#endif
}

inline bool isnan(_Scalar __x)
{
#ifdef __GNUC__
    return __builtin_isnan(__x);
#else
    return __x != __x;
#endif
}

inline bool isnormal(_Scalar __x)
{
#ifdef __GNUC__
    return __builtin_isnormal(__x);
#else
    _Scalar __reciprocal = 1 / __x;
    return __reciprocal && isfinite(__reciprocal);
#endif
}

inline bool signbit(_Scalar __x)
{
#ifdef __GNUC__
    return __builtin_signbit(__x);
#else
    return copysign(1, __x) < 0;
#endif
}

inline int fpclassify(_Scalar __x)
{
#ifdef __GNUC__
    return __builtin_fpclassify(FP_NAN, FP_INFINITE, FP_NORMAL, FP_SUBNORMAL, FP_ZERO, __x);
#else
    if (isnan(__x))    return FP_NAN;
    if (isinf(__x))    return FP_INFINITE;
    if (isnormal(__x)) return FP_NORMAL;
    return __x ? FP_SUBNORMAL : FP_ZERO;
#endif
}

inline bool isgreater(_Scalar __x, _Scalar __y)
{
#ifdef __GNUC__
    return __builtin_isgreater(__x, __y);
#else
    return __x > __y;
#endif
}

inline bool isgreaterequal(_Scalar __x, _Scalar __y)
{
#ifdef __GNUC__
    return __builtin_isgreaterequal(__x, __y);
#else
    return __x >= __y;
#endif
}

inline bool isless(_Scalar __x, _Scalar __y)
{
#ifdef __GNUC__
    return __builtin_isless(__x, __y);
#else
    return __x < __y;
#endif
}

inline bool islessequal(_Scalar __x, _Scalar __y)
{
#ifdef __GNUC__
    return __builtin_islessequal(__x, __y);
#else
    return __x <= __y;
#endif
}

inline bool islessgreater(_Scalar __x, _Scalar __y)
{
#ifdef __GNUC__
    return __builtin_islessgreater(__x, __y);
#else
    return __x < __y || __x > __y;
#endif
}

inline bool isunordered(_Scalar __x, _Scalar __y)
{
#ifdef __GNUC__
    return __builtin_islessgreater(__x, __y);
#else
    return __x != __x || __y != __y;
#endif
}

} // extern "C++"

#endif /* C++11 */

#undef _MATHCALL

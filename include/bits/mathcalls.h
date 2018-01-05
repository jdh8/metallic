/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2017 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#define _CAT2(a, b)  a##b
#define _CAT(a, b)   _CAT2(a, b)
#define _MATHCALL(f) _CAT(f, _SUFFIX)

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

_Scalar _MATHCALL(frexp)(_Scalar, int[_STATIC 1]);
_Scalar _MATHCALL(ldexp)(_Scalar, int);

#if __STDC_VERSION__ >= 199901L || __cplusplus >= 201103L
_Scalar _MATHCALL(remainder)(_Scalar, _Scalar);
_Scalar _MATHCALL(remquo)(_Scalar, _Scalar, int[_STATIC 1]);
_Scalar _MATHCALL(fma)(_Scalar, _Scalar, _Scalar);
_Scalar _MATHCALL(nan)(const char[_STATIC 1]);

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

long      _MATHCALL(lround)(_Scalar __x);
long long _MATHCALL(llround)(_Scalar __x);

_Scalar   _MATHCALL(rint)(_Scalar __x);
long      _MATHCALL(lrint)(_Scalar __x);
long long _MATHCALL(llrint)(_Scalar __x);

_Scalar _MATHCALL(scalbn)(_Scalar, int);
_Scalar _MATHCALL(scalbln)(_Scalar, long);
int     _MATHCALL(ilogb)(_Scalar);
_Scalar _MATHCALL(logb)(_Scalar);
_Scalar _MATHCALL(nextafter)(_Scalar, _Scalar);
_Scalar _MATHCALL(nexttoward)(_Scalar, long double);
_Scalar _MATHCALL(copysign)(_Scalar, _Scalar);

inline _Scalar _MATHCALL(fmax)(_Scalar __x, _Scalar __y)
{
    return __x >= __y || __y != __y ? __x : __y;
}

inline _Scalar _MATHCALL(fmin)(_Scalar __x, _Scalar __y)
{
    return __x <= __y || __y != __y ? __x : __y;
}

inline _Scalar _MATHCALL(fdim)(_Scalar __x, _Scalar __y)
{
    return __x <= __y ? 0 : __x - __y;
}

inline _Scalar _MATHCALL(modf)(_Scalar __x, _Scalar __i[_STATIC 1])
{
    *__i = _MATHCALL(trunc)(__x);
    return __x - *__i;
}

inline _Scalar _MATHCALL(round)(_Scalar __x)
{
    return _MATHCALL(trunc)(__x + _MATHCALL(copysign)(0.5, __x));
}
#else
_Scalar _MATHCALL(modf)(_Scalar, _Scalar*);
#endif

/* vim: set ft=c: */

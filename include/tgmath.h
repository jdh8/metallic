// This file is part of Metallic, a runtime library for WebAssembly.
//
// Copyright (C) 2017 Chen-Pang He <chen.pang.he@jdh8.org>
//
// This Source Code Form is subject to the terms of the Mozilla
// Public License v. 2.0. If a copy of the MPL was not distributed
// with this file, You can obtain one at http://mozilla.org/MPL/2.0/

#ifndef _TGMATH_H
#define _TGMATH_H

#include <math.h>
#include <complex.h>

#if __STDC_VERSION__ >= 201112L
#define _TGMATH(x, function) _Generic(x,         \
    float: function##f,                          \
    default: function,                           \
    long double: function##l,                    \
    float _Complex: c##function##f,              \
    double _Complex: c##function,                \
    long double _Complex: c##function##l         \
)
#define _TGMATH_REAL(x, function) _Generic(x,    \
    float: function##f,                          \
    default: function,                           \
    long double: function##l                     \
)
#define _TGMATH_COMPLEX(x, function) _Generic((x + _Complex_I), \
    float _Complex: function##f,                                \
    default: function,                                          \
    long double _Complex: function##l                           \
)
#define fabs(x) _Generic(x,     \
    float: fabsf,               \
    default: fabs,              \
    long double: fabsl,         \
    float _Complex: cabsf,      \
    double _Complex: cabs,      \
    long double _Complex: cabsl \
)
#elif __GNUC__ * 100 + __GNUC_MINOR__ >= 301
#define _TGMATH(x, function) __builtin_choose_expr(                                                     \
    __builtin_types_compatible_p(__typeof__(x), float), function##f, __builtin_choose_expr(             \
    __builtin_types_compatible_p(__typeof__(x), long double), function##l, __builtin_choose_expr(       \
    __builtin_types_compatible_p(__typeof__(x), float _Complex), c##function##f, __builtin_choose_expr( \
    __builtin_types_compatible_p(__typeof__(x), double _Complex), c##function, __builtin_choose_expr(   \
    __builtin_types_compatible_p(__typeof__(x), long double _Complex), c##function##l, function         \
)))))
#define _TGMATH_REAL(x, function) __builtin_choose_expr(                                    \
    __builtin_types_compatible_p(__typeof__(x), float), function##f, __builtin_choose_expr( \
    __builtin_types_compatible_p(__typeof__(x), long double), function##l, function         \
))
#define _TGMATH_COMPLEX(x, function) __builtin_choose_expr(                                                       \
    __builtin_types_compatible_p(__typeof__(x + _Complex_I), float _Complex), function##f, __builtin_choose_expr( \
    __builtin_types_compatible_p(__typeof__(x + _Complex_I), long double _Complex), function##l, function         \
))
#define fabs(x) __builtin_choose_expr(                                                         \
    __builtin_types_compatible_p(__typeof__(x), float), fabsf, __builtin_choose_expr(          \
    __builtin_types_compatible_p(__typeof__(x), long double), fabsl, __builtin_choose_expr(    \
    __builtin_types_compatible_p(__typeof__(x), float _Complex), cabsf, __builtin_choose_expr( \
    __builtin_types_compatible_p(__typeof__(x), double _Complex), cabs, __builtin_choose_expr( \
    __builtin_types_compatible_p(__typeof__(x), long double _Complex), cabsl, fabs             \
)))))
#else
#error This tgmath.h implementation depends on C11 or GCC >= 3.1.
#endif

#define exp(x) _TGMATH(x, exp)(x)
#define log(x) _TGMATH(x, log)(x)
#define pow(x, y) _TGMATH((x)*(y), pow)(x, y)
#define sqrt(x) _TGMATH(x, sqrt)(x)
#define sin(x) _TGMATH(x, sin)(x)
#define cos(x) _TGMATH(x, cos)(x)
#define tan(x) _TGMATH(x, tan)(x)
#define asin(x) _TGMATH(x, asin)(x)
#define acos(x) _TGMATH(x, acos)(x)
#define atan(x) _TGMATH(x, atan)(x)
#define sinh(x) _TGMATH(x, sinh)(x)
#define cosh(x) _TGMATH(x, cosh)(x)
#define tanh(x) _TGMATH(x, tanh)(x)
#define asinh(x) _TGMATH(x, asinh)(x)
#define acosh(x) _TGMATH(x, acosh)(x)
#define atanh(x) _TGMATH(x, atanh)(x)

#define fmod(x, y) _TGMATH_REAL((x)/(y), fmod)(x, y)
#define remainder(x, y) _TGMATH_REAL((x)/(y), remainder)(x, y)
#define remquo(x, y, q) _TGMATH_REAL((x)/(y), remquo)(x, y, q)
#define fma(x, y, a) _TGMATH_REAL((x)*(y)+(a), fma)(x, y, a)
#define fmax(x, y) _TGMATH_REAL((x)-(y), fmax)(x, y)
#define fmin(x, y) _TGMATH_REAL((x)-(y), fmin)(x, y)
#define fdim(x, y) _TGMATH_REAL((x)-(y), fdim)(x, y)
#define exp2(x) _TGMATH_REAL(x, exp2)(x)
#define expm1(x) _TGMATH_REAL(x, expm1)(x)
#define log10(x) _TGMATH_REAL(x, log10)(x)
#define log2(x) _TGMATH_REAL(x, log2)(x)
#define log1p(x) _TGMATH_REAL(x, log1p)(x)
#define cbrt(x) _TGMATH_REAL(x, cbrt)(x)
#define hypot(x, y) _TGMATH_REAL((x)+(y), hypot)(x, y)
#define atan2(y, x) _TGMATH_REAL((y)/(x), atan2)(y, x)
#define erf(x) _TGMATH_REAL(x, erf)(x)
#define erfc(x) _TGMATH_REAL(x, erfc)(x)
#define tgamma(x) _TGMATH_REAL(x, tgamma)(x)
#define lgamma(x) _TGMATH_REAL(x, lgamma)(x)
#define ceil(x) _TGMATH_REAL(x, ceil)(x)
#define floor(x) _TGMATH_REAL(x, floor)(x)
#define trunc(x) _TGMATH_REAL(x, trunc)(x)
#define nearbyint(x) _TGMATH_REAL(x, nearbyint)(x)
#define round(x) _TGMATH_REAL(x, round)(x)
#define lround(x) _TGMATH_REAL(x, llround)(x)
#define llround(x) _TGMATH_REAL(x, llround)(x)
#define rint(x) _TGMATH_REAL(x, rint)(x)
#define lrint(x) _TGMATH_REAL(x, llrint)(x)
#define llrint(x) _TGMATH_REAL(x, llrint)(x)
#define frexp(x, e) _TGMATH_REAL(x, frexp)(x, e)
#define ldexp(x, e) _TGMATH_REAL(x, ldexp)(x, e)
#define scalbn(x, e) _TGMATH_REAL(x, scalbn)(x, e)
#define scalbln(x, e) _TGMATH_REAL(x, scalbln)(x, e)
#define ilogb(x) _TGMATH_REAL(x, ilogb)(x)
#define logb(x) _TGMATH_REAL(x, logb)(x)
#define nextafter(x, y) _TGMATH_REAL((x)+(y), nextafter)(x, y)
#define nexttoward(x, y) _TGMATH_REAL((x)+(y), nexttoward)(x, y)
#define copysign(x, y) _TGMATH_REAL((x)+(y), copysign)(x, y)

#define carg(z) _TGMATH_COMPLEX(z, carg)(z)
#define conj(z) _TGMATH_COMPLEX(z, conj)(z)
#define creal(z) _TGMATH_COMPLEX(z, creal)(z)
#define cimag(z) _TGMATH_COMPLEX(z, cimag)(z)
#define cproj(z) _TGMATH_COMPLEX(z, cproj)(z)

#endif // tgmath.h

// vim: ft=c

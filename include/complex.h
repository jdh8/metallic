// This file is part of Metallic, a runtime library for WebAssembly.
//
// Copyright (C) 2017 Chen-Pang He <chen.pang.he@jdh8.org>
//
// This Source Code Form is subject to the terms of the Mozilla
// Public License v. 2.0. If a copy of the MPL was not distributed
// with this file, You can obtain one at http://mozilla.org/MPL/2.0/

#ifndef _COMPLEX_H
#define _COMPLEX_H

#if __STDC_VERSION__ >= 199901L
#define complex _Complex
#define _Complex_I (__extension__ 1.0if)
#define I _Complex_I
#elif __cplusplus
extern "C" {
#endif
float crealf(float _Complex);
double creal(double _Complex);
long double creall(long double _Complex);

float cimagf(float _Complex);
double cimag(double _Complex);
long double cimagl(long double _Complex);

float cabsf(float _Complex);
double cabs(double _Complex);
long double cabsl(long double _Complex);

float cargf(float _Complex);
double carg(double _Complex);
long double cargl(long double _Complex);

float _Complex conjf(float _Complex);
double _Complex conj(double _Complex);
long double _Complex conjl(long double _Complex);

float _Complex cprojf(float _Complex);
double _Complex cproj(double _Complex);
long double _Complex cprojl(long double _Complex);

float _Complex cexpf(float _Complex);
double _Complex cexp(double _Complex);
long double _Complex cexpl(long double _Complex);

float _Complex clogf(float _Complex);
double _Complex clog(double _Complex);
long double _Complex clogl(long double _Complex);

float _Complex cpowf(float _Complex, float _Complex);
double _Complex cpow(double _Complex, double _Complex);
long double _Complex cpowl(long double _Complex, long double _Complex);

float _Complex csqrtf(float _Complex);
double _Complex csqrt(double _Complex);
long double _Complex csqrtl(long double _Complex);

float _Complex csinf(float _Complex);
double _Complex csin(double _Complex);
long double _Complex csinl(long double _Complex);

float _Complex ccosf(float _Complex);
double _Complex ccos(double _Complex);
long double _Complex ccosl(long double _Complex);

float _Complex ctanf(float _Complex);
double _Complex ctan(double _Complex);
long double _Complex ctanl(long double _Complex);

float _Complex casinf(float _Complex);
double _Complex casin(double _Complex);
long double _Complex casinl(long double _Complex);

float _Complex cacosf(float _Complex);
double _Complex cacos(double _Complex);
long double _Complex cacosl(long double _Complex);

float _Complex catanf(float _Complex);
double _Complex catan(double _Complex);
long double _Complex catanl(long double _Complex);

float _Complex csinhf(float _Complex);
double _Complex csinh(double _Complex);
long double _Complex csinhl(long double _Complex);

float _Complex ccoshf(float _Complex);
double _Complex ccosh(double _Complex);
long double _Complex ccoshl(long double _Complex);

float _Complex ctanhf(float _Complex);
double _Complex ctanh(double _Complex);
long double _Complex ctanhl(long double _Complex);

float _Complex casinhf(float _Complex);
double _Complex casinh(double _Complex);
long double _Complex casinhl(long double _Complex);

float _Complex cacoshf(float _Complex);
double _Complex cacosh(double _Complex);
long double _Complex cacoshl(long double _Complex);

float _Complex catanhf(float _Complex);
double _Complex catanh(double _Complex);
long double _Complex catanhl(long double _Complex);
#ifdef __cplusplus
} // extern "C"
#endif

#ifdef __clang__
#define __CMPLX(T, x, y) (T _Complex){ x, y }
#elif __GNUC__
#define __CMPLX(T, x, y) __builtin_complex((T)(x), (T)(y))
#else
#define __CMPLX(T, x, y) (union { T _Complex __z; T __xy[2]; }){.__xy = {(x), (y)}}.__z
#endif

#if __STDC_VERSION__ >= 201112L
#define CMPLXF(x, y) __CMPLX(float, x, y)
#define CMPLX(x, y)  __CMPLX(double, x, y)
#define CMPLXL(x, y) __CMPLX(long double, x, y)
#endif

#endif // complex.h

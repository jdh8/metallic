// This file is part of Metallic, a runtime library for WebAssembly.
//
// Copyright (C) 2017 Chen-Pang He <chen.pang.he@jdh8.org>
//
// This Source Code Form is subject to the terms of the Mozilla
// Public License v. 2.0. If a copy of the MPL was not distributed
// with this file, You can obtain one at http://mozilla.org/MPL/2.0/

#define _CAT2(a, b)  a##b
#define _CAT(a, b)   _CAT2(a, b)
#define _MATHCALL(f) _CAT(f, _SUFFIX)

#ifdef __cplusplus
#define _ComplexScalar std::complex<_RealScalar>
#else
#define _ComplexScalar _RealScalar _Complex

_RealScalar _MATHCALL(creal)(_ComplexScalar);
_RealScalar _MATHCALL(cimag)(_ComplexScalar);
_ComplexScalar _MATHCALL(conj)(_ComplexScalar);
_ComplexScalar _MATHCALL(cproj)(_ComplexScalar);

_RealScalar _MATHCALL(hypot)(_RealScalar, _RealScalar);
_RealScalar _MATHCALL(atan2)(_RealScalar, _RealScalar);

inline _RealScalar _MATHCALL(cabs)(_ComplexScalar __z)
{
    return _MATHCALL(hypot)(_MATHCALL(creal)(__z), _MATHCALL(cimag)(__z));
}

inline _RealScalar _MATHCALL(carg)(_ComplexScalar __z)
{
    return _MATHCALL(atan2)(_MATHCALL(cimag)(__z), _MATHCALL(creal)(__z));
}
#endif // __cplusplus

_ComplexScalar _MATHCALL(cexp)(_ComplexScalar);
_ComplexScalar _MATHCALL(clog)(_ComplexScalar);
_ComplexScalar _MATHCALL(cpow)(_ComplexScalar, _ComplexScalar);
_ComplexScalar _MATHCALL(csqrt)(_ComplexScalar);
_ComplexScalar _MATHCALL(csin)(_ComplexScalar);
_ComplexScalar _MATHCALL(ccos)(_ComplexScalar);
_ComplexScalar _MATHCALL(ctan)(_ComplexScalar);
_ComplexScalar _MATHCALL(casin)(_ComplexScalar);
_ComplexScalar _MATHCALL(cacos)(_ComplexScalar);
_ComplexScalar _MATHCALL(catan)(_ComplexScalar);
_ComplexScalar _MATHCALL(csinh)(_ComplexScalar);
_ComplexScalar _MATHCALL(ccosh)(_ComplexScalar);
_ComplexScalar _MATHCALL(ctanh)(_ComplexScalar);
_ComplexScalar _MATHCALL(casinh)(_ComplexScalar);
_ComplexScalar _MATHCALL(cacosh)(_ComplexScalar);
_ComplexScalar _MATHCALL(catanh)(_ComplexScalar);

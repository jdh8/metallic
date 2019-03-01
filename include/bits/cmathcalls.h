#define _CAT2(a, b)    a##b
#define _CAT(a, b)     _CAT2(a, b)
#define _MATHCALL(f)   _CAT(f, _SUFFIX)
#define _ComplexScalar _RealScalar _Complex

_RealScalar _MATHCALL(creal)(_ComplexScalar);
_RealScalar _MATHCALL(cimag)(_ComplexScalar);
_RealScalar _MATHCALL(cabs)(_ComplexScalar);
_RealScalar _MATHCALL(carg)(_ComplexScalar);
_ComplexScalar _MATHCALL(conj)(_ComplexScalar);
_ComplexScalar _MATHCALL(cproj)(_ComplexScalar);

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

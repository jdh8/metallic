#include "binary.h"

double cimag(double _Complex);

static inline _Bool cidentical(float _Complex x, float _Complex y)
{
    return reinterpret(uint64_t, x) == reinterpret(uint64_t, y);
}

static inline _Bool capprox(double _Complex x, double _Complex y, double re, double im)
{
    return approx(x, y, re) && approx(cimag(x), cimag(y), im);
}

#include "binary.h"

float cimagf(float _Complex);

static inline _Bool cidentical(float _Complex x, float _Complex y)
{
    return reinterpret(uint64_t, x) == reinterpret(uint64_t, y);
}

static inline _Bool capprox(float _Complex x, float _Complex y, unsigned re, unsigned im)
{
    return approx(x, y, re) && approx(cimagf(x), cimagf(y), im);
}

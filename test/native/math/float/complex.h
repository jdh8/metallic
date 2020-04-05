#include "binary.h"

float cimagf(float _Complex);

static inline _Bool cidentical(float _Complex x, float _Complex y)
{
    return reinterpret(uint64_t, x) == reinterpret(uint64_t, y);
}

static inline _Bool approx(float x, float y, unsigned tolerance)
{
    uint32_t a = reinterpret(uint32_t, x);
    uint32_t b = reinterpret(uint32_t, y);

    return a - b + tolerance <= 2 * tolerance;
}

static inline _Bool capprox(float _Complex x, float _Complex y, unsigned re, unsigned im)
{
    return approx(x, y, re) && approx(cimagf(x), cimagf(y), im);
}

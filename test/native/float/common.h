#include "../../../src/math/reinterpret.h"
#include <stdint.h>
#include <float.h>
#include <inttypes.h>
#include <stdio.h>

static inline _Bool approx(double x, double y)
{
    const uint64_t mask = (1L << (DBL_MANT_DIG - FLT_MANT_DIG)) - 1;
    uint64_t a = reinterpret(uint64_t, x);
    uint64_t b = reinterpret(uint64_t, y);

    return a - b + mask <= 2 * mask;
}

static inline _Bool approxf(float x, float y)
{
    uint32_t a = reinterpret(uint32_t, x);
    uint32_t b = reinterpret(uint32_t, y);

    return a - b + 1 <= 2;
}

double cimag(double _Complex);

static inline _Bool capprox(double _Complex x, double _Complex y)
{
    return approx(x, y) && approx(cimag(x), cimag(y));
}

static inline _Bool identical(float x, float y)
{
    return reinterpret(uint32_t, x) == reinterpret(uint32_t, y);
}

_Noreturn void abort(void);

#define verify(cond, x) if (!(cond)) {                                                               \
    float y = x;                                                                                     \
    fprintf(stderr, "Assertion `"#cond"' failed at %g (%#"PRIx32")\n", y, reinterpret(uint32_t, y)); \
    abort();                                                                                         \
}

/* vim: set ft=c: */

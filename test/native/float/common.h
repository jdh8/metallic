#include "../../../src/math/reinterpret.h"
#include <stdint.h>
#include <float.h>
#include <inttypes.h>
#include <stdio.h>

float cimagf(float _Complex);

static inline _Bool identical(float x, float y)
{
    return reinterpret(uint32_t, x) == reinterpret(uint32_t, y);
}

static inline _Bool faithful(double x, double y)
{
    const uint64_t mask = (1L << (DBL_MANT_DIG - FLT_MANT_DIG)) - 1;
    uint64_t a = reinterpret(uint64_t, x);
    uint64_t b = reinterpret(uint64_t, y);

    return a - b + mask <= 2 * mask;
}

static inline _Bool approx(float x, float y)
{
    uint32_t a = reinterpret(uint32_t, x);
    uint32_t b = reinterpret(uint32_t, y);

    return a - b + 1 <= 2;
}

static inline _Bool capprox(float _Complex x, float _Complex y)
{
    return approx(x, y) && approx(cimagf(x), cimagf(y));
}

_Noreturn void abort(void);

#define verify(cond, x) if (!(cond)) {                                                               \
    float y = x;                                                                                     \
    fprintf(stderr, "Assertion `"#cond"' failed at %g (%#"PRIx32")\n", y, reinterpret(uint32_t, y)); \
    abort();                                                                                         \
}

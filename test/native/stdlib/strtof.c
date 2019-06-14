#include "src/stdlib/parse/scientificf.h"
#include <stdint.h>

typedef float Scalar;
typedef uint32_t Bitset;

#include "src/stdlib/parse/float.h"
#include "math/float/unary.h"
#include <assert.h>

static void f(const char format[static 1], float x)
{
    char s[32];
    sprintf(s, format, x);
    verify(x == parsefloat_(s, (void*)0), x);

    *s = '+';
    verify(x == parsefloat_(s, (void*)0), x);

    *s = '-';
    verify(-x == parsefloat_(s, (void*)0), x);
}

static void run(float x)
{
    f(" %.9g", x);
    f(" %a", x);
}

int main(void)
{
    for (uint32_t i = 0; i < 0x7F800000; i += 1337)
        run(reinterpret(float, i));

    assert(parsefloat_("inf", (void*)0) == INFINITY);
    assert(parsefloat_("+infinity", (void*)0) == INFINITY);
    assert(parsefloat_("-infiltration", (void*)0) == -INFINITY);

    assert(isnan(parsefloat_("nan", (void*)0)));
    assert(isnan(parsefloat_("nanana", (void*)0)));
    assert(isnan(parsefloat_("nani(3)", (void*)0)));
    assert(isnan(parsefloat_("nan(3)", (void*)0)));
    assert(isnan(parsefloat_("nan(", (void*)0)));
}

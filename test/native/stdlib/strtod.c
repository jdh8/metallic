#include "src/stdlib/parse/scientific.h"
#include <stdint.h>

typedef double Scalar;
typedef uint64_t Bitset;

#include "src/stdlib/parse/float.h"
#include "math/double/unary.h"
#include <assert.h>

static void f(const char format[static 1], double x)
{
    char s[32];
    sprintf(s, format, x);
    verify(x == parsefloat_(s, (void*)0), x);

    *s = '+';
    verify(x == parsefloat_(s, (void*)0), x);

    *s = '-';
    verify(-x == parsefloat_(s, (void*)0), x);
}

static void run(double x)
{
    f(" %.17g", x);
    f(" %a", x);
}

int main(void)
{
    for (uint64_t i = 0; i < 0x7FF0000000000000; i += 0x000007DEB645A66A)
        run(reinterpret(double, i));

    assert(parsefloat_("inf", (void*)0) == INFINITY);
    assert(parsefloat_("+infinity", (void*)0) == INFINITY);
    assert(parsefloat_("-infiltration", (void*)0) == -INFINITY);

    assert(isnan(parsefloat_("nan", (void*)0)));
    assert(isnan(parsefloat_("nanana", (void*)0)));
    assert(isnan(parsefloat_("nani(3)", (void*)0)));
    assert(isnan(parsefloat_("nan(3)", (void*)0)));
    assert(isnan(parsefloat_("nan(", (void*)0)));
}

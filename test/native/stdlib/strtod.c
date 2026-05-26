#include "src/stdlib/parse/scientific.h"
#include <stdint.h>

typedef double Scalar;
typedef uint64_t Bitset;

static double decimal_to_scalar_(const decimal_t* d)
{
    return decimal_to_double_(d);
}

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

    /* Issue #5 -- correctly-rounded boundary cases */
    assert(parsefloat_(".2470328229206232720882843964341106861825299013071623822127928412503377536351044e-323", (void*)0) == 0x1p-1074);
    assert(parsefloat_(".7410984687618698162648531893023320585475897039214871466383785237510132609053131e-323", (void*)0) == 0x1p-1074);
    assert(parsefloat_(
        "17976931348623158079372897140530341507993413271003782693617377898044"
        "4968292764750946649017977587207096330286416692887910946555547851940402630657488671"
        "5058206819089020007083836762738548458177115317644757302700698555713669596228429148"
        "1986083493647529271907416844436551070434271155969950809304288017790417449779200", (void*)0) == INFINITY);
}

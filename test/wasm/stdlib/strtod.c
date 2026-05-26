#include <assert.h>
#include <math.h>
#include <stdlib.h>
#include <errno.h>

#define RUN(s, junk) check(s junk, sizeof(s) - 1)

#define OVERFLOW(condition) do {  \
    errno = 0;                    \
    assert(condition);            \
    assert(errno == ERANGE);      \
} while (0)

static double check(const char s[static 1], long length)
{
    char* end;
    double x = strtod(s, &end);
    assert(end == s + length);
    return x;
}

int main(void)
{
    /* Specials */
    assert(RUN("inf", "") == INFINITY);
    assert(RUN("+inf", "ini") == INFINITY);
    assert(RUN("infinity", "") == INFINITY);
    assert(isnan(RUN("nan", "")));
    assert(isnan(RUN("nan(0xabcd)", "")));

    /* Simple values */
    assert(RUN("0", "") == 0);
    assert(signbit(RUN("-0", "x")));
    assert(RUN("3", "") == 3);
    assert(RUN("-.003", "f") == -0.003);
    assert(RUN("0x3", "") == 3);
    assert(RUN("0x1.8p1", "") == 3);

    /* Subnormal boundary */
    assert(RUN("4.9406564584124654e-324", "") == 0x1p-1074);            /* DBL_TRUE_MIN */
    assert(RUN("2.4703282292062327e-324", "") == 0);                    /* just below 0.5 * MIN, rounds to 0 */

    /* Issue #5: long-input halfway boundaries */
    assert(RUN(".2470328229206232720882843964341106861825299013071623822127928412503377536351044e-323", "") == 0x1p-1074);
    assert(RUN(".7410984687618698162648531893023320585475897039214871466383785237510132609053131e-323", "") == 0x1p-1074);

    /* Normal/subnormal junction */
    assert(RUN("2.2250738585072014e-308", "") == 0x1p-1022);             /* DBL_MIN */

    /* DBL_MAX and overflow */
    assert(RUN("1.7976931348623157e+308", "") == 0x1.fffffffffffffp+1023);
    OVERFLOW(RUN("1.7976931348623159e+308", "") == INFINITY);
    OVERFLOW(RUN(
        "17976931348623158079372897140530341507993413271003782693617377898044"
        "4968292764750946649017977587207096330286416692887910946555547851940402630657488671"
        "5058206819089020007083836762738548458177115317644757302700698555713669596228429148"
        "1986083493647529271907416844436551070434271155969950809304288017790417449779200", "") == INFINITY);
}

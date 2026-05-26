#include <assert.h>
#include <errno.h>
#include <float.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

#define RUN(s, junk) check(s junk, sizeof(s) - 1)

#define OVERFLOW(condition) do {  \
    errno = 0;                    \
    assert(condition);            \
    assert(errno == ERANGE);      \
} while (0)

static long double check(const char s[static 1], long length)
{
    char* end;
    long double x = strtold(s, &end);
    assert(end == s + length);
    return x;
}

static int bits_eq(long double a, long double b)
{
    return memcmp(&a, &b, sizeof a) == 0;
}

int main(void)
{
    /* Specials */
    assert(RUN("inf", "") == (long double)INFINITY);
    assert(RUN("+inf", "ini") == (long double)INFINITY);
    assert(RUN("infinity", "") == (long double)INFINITY);
    assert(isnan(RUN("nan", "")));
    assert(isnan(RUN("nan(0xabcd)", "")));

    /* Simple values */
    assert(RUN("0", "") == 0);
    assert(signbit(RUN("-0", "x")));
    assert(RUN("3", "") == 3);
    assert(RUN("-.003", "f") == -0.003L);
    assert(RUN("0x3", "") == 3);
    assert(RUN("0x1.8p1", "") == 3);

    /* Subnormal boundary (binary128 true_min == 2^-16494, 36-digit form) */
    assert(bits_eq(RUN("6.47517511943802511092443895822764655e-4966", ""), LDBL_TRUE_MIN));
    assert(RUN("3.2e-4966", "") == 0);  /* below 0.5 * LDBL_TRUE_MIN, rounds to 0 */

    /* Normal/subnormal junction (LDBL_MIN == 2^-16382, 36-digit form) */
    assert(bits_eq(RUN("3.36210314311209350626267781732175260e-4932", ""), LDBL_MIN));

    /* LDBL_MAX and overflow (36-digit LDBL_MAX, then halfway-and-above). */
    assert(bits_eq(RUN("1.18973149535723176508575932662800701e+4932", ""), LDBL_MAX));
    OVERFLOW(RUN("1.18973149535723176508575932662800708e+4932", "") == (long double)INFINITY);
    OVERFLOW(RUN("1e4933", "") == (long double)INFINITY);

    /* Long-input halfway boundaries (mirroring strtod's issue #5 tests).
     * 0.5 * LDBL_TRUE_MIN is exactly 2^-16495 with leading digits
     * 3.23758755971901255546221947911382327624978466901734050484944...
     * Both inputs have 51 digits, triggering the bigint slow path; the
     * 50th fractional digit selects which side of halfway we land on. */
    assert(bits_eq(RUN("3.23758755971901255546221947911382327624978466901735e-4966", ""), LDBL_TRUE_MIN));
    assert(RUN("3.23758755971901255546221947911382327624978466901733e-4966", "") == 0);

    /* Powers of two near common boundaries */
    assert(RUN("1.0", "") == 1.0L);
    assert(RUN("2.0", "") == 2.0L);
    assert(RUN("0.5", "") == 0.5L);
    assert(bits_eq(RUN("0x1p-16382", ""), LDBL_MIN));
    assert(bits_eq(RUN("0x1p-16494", ""), LDBL_TRUE_MIN));

    /* Round-trip a value that requires the full 113-bit mantissa
     * disambiguation: 1 + 2^-112 is the smallest binary128 strictly
     * greater than 1.0.  Its decimal is short. */
    long double next1 = 1.0L + 0x1p-112L;
    assert(next1 != 1.0L);
    assert(RUN("1.0000000000000000000000000000000002", "") == next1);
}

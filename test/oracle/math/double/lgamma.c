/* signgam is a POSIX extension used by lgamma_mpfr.c */
#define _GNU_SOURCE
#include <math.h>
#include "src/math/double/lgamma.c"
#include "binary64/lgamma/lgamma_mpfr.c"
#include "../../sample.h"

static double fut(double x) { return lgamma(x); }
static double ref(double x) { return ref_lgamma(x); }

int main(void)
{
    uint64_t bad = 0;

    ref_init();
    bad |= sample_wc_f64(fut, ref, CORE_MATH "/binary64/lgamma/lgamma.wc");
    bad |= sample_f64(fut, ref, 0, 10, 0, 20000000);
    bad |= sample_f64(fut, ref, -5, 0, 1, 10000000);
    bad |= sample_f64(fut, ref, -1074, -1, 1, 5000000);

    fputs(bad ? "FAIL\n" : "OK\n", stderr);
    return bad != 0;
}

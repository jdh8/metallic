#include <assert.h>
#include "identical.h"
#include <math.h>

int main(void)
{
    assert(identical(rsqrt(0.0), INFINITY));
    assert(identical(rsqrt(-0.0), -INFINITY));
    assert(identical(rsqrt(INFINITY), 0.0));
    assert(isnan(rsqrt(-1.0)));
    assert(isnan(rsqrt(-INFINITY)));
    assert(isnan(rsqrt(NAN)));

    /* x = 4^k is exact across the whole range, subnormals included. */
    assert(identical(rsqrt(1.0), 1.0));
    assert(identical(rsqrt(4.0), 0.5));
    assert(identical(rsqrt(0x1p100), 0x1p-50));
    assert(identical(rsqrt(0x1p-1074), 0x1p537));
    assert(identical(rsqrt(0x1p-1022), 0x1p511));

    /* Correctly rounded samples (MPFR-derived). */
    assert(identical(rsqrt(0x1.fffffffffffffp+1023), 0x1p-512));
    assert(identical(rsqrt(2.0), 0x1.6a09e667f3bcdp-1));
    assert(identical(rsqrt(3.0), 0x1.279a74590331cp-1));
    assert(identical(rsqrt(0.5), 0x1.6a09e667f3bcdp+0));
    assert(identical(rsqrt(10.0), 0x1.43d136248490fp-2));
    assert(identical(rsqrt(100.0), 0x1.999999999999ap-4));
}

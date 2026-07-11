#include <assert.h>
#include "identical.h"
#include <math.h>

int main(void)
{
    assert(identical(log2p1(0.0), 0.0));
    assert(identical(log2p1(-0.0), -0.0));
    assert(identical(log2p1(-1.0), -INFINITY));
    assert(isnan(log2p1(-2.0)));
    assert(isnan(log2p1(-INFINITY)));
    assert(identical(log2p1(INFINITY), INFINITY));
    assert(isnan(log2p1(NAN)));

    /* log2p1(2^k - 1) = k exactly. */
    assert(identical(log2p1(1.0), 1.0));
    assert(identical(log2p1(3.0), 2.0));
    assert(identical(log2p1(7.0), 3.0));
    assert(identical(log2p1(1023.0), 10.0));
    assert(identical(log2p1(0x1p53 - 1.0), 53.0));
    assert(identical(log2p1(-0.5), -1.0));
    assert(identical(log2p1(-0.75), -2.0));
}

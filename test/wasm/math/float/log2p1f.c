#include <assert.h>
#include "identical.h"
#include <math.h>

int main(void)
{
    assert(identical(log2p1f(0.0f), 0.0f));
    assert(identical(log2p1f(-0.0f), -0.0f));
    assert(identical(log2p1f(-1.0f), -INFINITY));
    assert(isnan(log2p1f(-2.0f)));
    assert(isnan(log2p1f(-INFINITY)));
    assert(identical(log2p1f(INFINITY), INFINITY));
    assert(isnan(log2p1f(NAN)));

    /* log2p1(2^k - 1) = k exactly. */
    assert(identical(log2p1f(1.0f), 1.0f));
    assert(identical(log2p1f(3.0f), 2.0f));
    assert(identical(log2p1f(7.0f), 3.0f));
    assert(identical(log2p1f(0x1p20f - 1.0f), 20.0f));
    assert(identical(log2p1f(-0.5f), -1.0f));
    assert(identical(log2p1f(-0.75f), -2.0f));

    /* The sweep-pinned tie. */
    assert(identical(log2p1f(0x1.7a13c6p+30f), 0x1.e90026p+4f));
}

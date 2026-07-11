#include <assert.h>
#include "identical.h"
#include <math.h>

int main(void)
{
    /* Zero y: +-0 in the right half-plane, +-1 in the left. */
    assert(identical(atan2pif(0.0f, 1.0f), 0.0f));
    assert(identical(atan2pif(-0.0f, 1.0f), -0.0f));
    assert(identical(atan2pif(0.0f, -1.0f), 1.0f));
    assert(identical(atan2pif(-0.0f, -1.0f), -1.0f));
    assert(identical(atan2pif(0.0f, 0.0f), 0.0f));
    assert(identical(atan2pif(0.0f, -0.0f), 1.0f));
    assert(identical(atan2pif(-0.0f, -0.0f), -1.0f));

    /* Axes and infinite corners: exact half-turn rays. */
    assert(identical(atan2pif(1.0f, 0.0f), 0.5f));
    assert(identical(atan2pif(-1.0f, 0.0f), -0.5f));
    assert(identical(atan2pif(1.0f, -0.0f), 0.5f));
    assert(identical(atan2pif(INFINITY, 1.0f), 0.5f));
    assert(identical(atan2pif(-INFINITY, 1.0f), -0.5f));
    assert(identical(atan2pif(INFINITY, INFINITY), 0.25f));
    assert(identical(atan2pif(INFINITY, -INFINITY), 0.75f));
    assert(identical(atan2pif(-INFINITY, -INFINITY), -0.75f));
    assert(identical(atan2pif(1.0f, INFINITY), 0.0f));
    assert(identical(atan2pif(-1.0f, INFINITY), -0.0f));
    assert(identical(atan2pif(1.0f, -INFINITY), 1.0f));
    assert(identical(atan2pif(-1.0f, -INFINITY), -1.0f));

    /* Diagonals: exact quarters. */
    assert(identical(atan2pif(1.0f, 1.0f), 0.25f));
    assert(identical(atan2pif(-1.0f, 1.0f), -0.25f));
    assert(identical(atan2pif(1.0f, -1.0f), 0.75f));
    assert(identical(atan2pif(-1.0f, -1.0f), -0.75f));

    assert(isnan(atan2pif(NAN, 1.0f)));
    assert(isnan(atan2pif(1.0f, NAN)));
}

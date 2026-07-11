#include <assert.h>
#include "identical.h"
#include <math.h>

int main(void)
{
    assert(identical(exp10(0.0), 1.0));
    assert(identical(exp10(-0.0), 1.0));
    assert(identical(exp10(1.0), 10.0));
    assert(identical(exp10(2.0), 100.0));
    assert(identical(exp10(22.0), 1e22)); /* the last exactly-representable power */
    assert(identical(exp10(-1.0), 0.1));
    assert(identical(exp10(INFINITY), INFINITY));
    assert(identical(exp10(-INFINITY), 0.0));
    assert(isnan(exp10(NAN)));

    /* Thresholds, exact to the ulp (last finite input -> ~DBL_MAX). */
    assert(identical(exp10(0x1.34413509f79fep+8), 0x1.ffffffffffba1p+1023));
    assert(identical(exp10(0x1.34413509f79ffp+8), INFINITY));
    assert(identical(exp10(-324.0), 0.0));
}

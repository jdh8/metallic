#include <assert.h>
#include "identical.h"
#include <math.h>

int main(void)
{
    assert(identical(exp10m1(0.0), 0.0));
    assert(identical(exp10m1(-0.0), -0.0));
    assert(identical(exp10m1(INFINITY), INFINITY));
    assert(identical(exp10m1(-INFINITY), -1.0));
    assert(isnan(exp10m1(NAN)));

    assert(identical(exp10m1(1.0), 9.0));
    assert(identical(exp10m1(2.0), 99.0));
    assert(identical(exp10m1(3.0), 999.0));
    assert(identical(exp10m1(-1.0), -0.9));

    assert(identical(exp10m1(0x1.34413509f79ffp+8), INFINITY));
    assert(identical(exp10m1(-17.0), -1.0));
}

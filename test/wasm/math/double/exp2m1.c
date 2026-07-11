#include <assert.h>
#include "identical.h"
#include <math.h>

int main(void)
{
    assert(identical(exp2m1(0.0), 0.0));
    assert(identical(exp2m1(-0.0), -0.0));
    assert(identical(exp2m1(INFINITY), INFINITY));
    assert(identical(exp2m1(-INFINITY), -1.0));
    assert(isnan(exp2m1(NAN)));

    assert(identical(exp2m1(1.0), 1.0));
    assert(identical(exp2m1(2.0), 3.0));
    assert(identical(exp2m1(3.0), 7.0));
    assert(identical(exp2m1(10.0), 1023.0));
    assert(identical(exp2m1(-1.0), -0.5));
    assert(identical(exp2m1(-2.0), -0.75));

    assert(identical(exp2m1(1024.0), INFINITY));
    assert(identical(exp2m1(-54.0), -1.0));
    assert(identical(exp2m1(-1000.0), -1.0));
}

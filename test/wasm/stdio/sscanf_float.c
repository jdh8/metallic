#include <stdio.h>
#include <math.h>
#include <assert.h>

int main(void)
{
    float f;
    double d;

    assert(sscanf("3.14", "%f", &f) == 1);
    assert(f > 3.13f && f < 3.15f);

    assert(sscanf("-2.5e3", "%lf", &d) == 1);
    assert(d == -2500.0);

    assert(sscanf("1.5e10", "%lf", &d) == 1);
    assert(d == 1.5e10);

    /* Hex float. */
    assert(sscanf("0x1.8p+4", "%lf", &d) == 1);
    assert(d == 24.0);

    /* INF/NAN. */
    assert(sscanf("inf", "%lf", &d) == 1);
    assert(isinf(d) && d > 0);

    assert(sscanf("-INFINITY", "%lf", &d) == 1);
    assert(isinf(d) && d < 0);

    assert(sscanf("NaN", "%lf", &d) == 1);
    assert(isnan(d));

    /* Width caps. */
    assert(sscanf("123.456", "%4lf", &d) == 1);
    assert(d == 123.0);

    /* Trailing 'e' without digits backs off. */
    int rest;
    assert(sscanf("1e abc", "%lf%n", &d, &rest) == 1);
    assert(d == 1.0);
    assert(rest == 1);

    return 0;
}

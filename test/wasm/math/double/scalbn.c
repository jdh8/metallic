#include "src/math/reinterpret.h"
#include <assert.h>
#include "identical.h"
#include <math.h>
#include <stdint.h>

static void up(double x)
{
    double y = x;

    for (int i = 0; i < 2500; ++i) {
        assert(scalbn(x, i) == y);
        y *= 2;
    }
}

static void down(double x)
{
    double coeff = 0.5;

    for (int i = -1; i > -1075; --i) {
        assert(identical(scalbn(x, i), x * coeff));
        coeff *= 0.5;
    }

    coeff = 0x1p-75;

    for (int i = -1075; i > -2075; --i) {
        assert(identical(scalbn(x, i), x * 0x1p-1000 * coeff));
        coeff *= 0.5;
    }

    coeff = 0x1p-75;

    for (int i = -2075; i > -2500; --i) {
        assert(identical(scalbn(x, i), x * 0x1p-1000 * 0x1p-1000 * coeff));
        coeff *= 0.5;
    }
}

static void run(double x)
{
    up(x);
    up(-x);
    down(x);
    down(-x);
}

int main(void)
{
    run(INFINITY);

    for (int64_t i = 0; i < 0x7FF0000000000000; i += 0x0000C649282A90E9)
        run(reinterpret(double, i));

    for (uint64_t i = 0x7FF8000000000000; i < 0x8000000000000000; i += 0x0000F4D6E9D6F44C) {
        double x = reinterpret(double, i);
        assert(isnan(scalbn(x, -37)));
        assert(isnan(scalbn(-x, -37)));
    }
}

#include "src/math/reinterpret.h"
#include <assert.h>
#include "identical.h"
#include <math.h>
#include <stdint.h>

static void up(float x)
{
    float y = x;

    for (int i = 0; i < 300; ++i) {
        assert(scalbnf(x, i) == y);
        y *= 2;
    }
}

static void down(float x)
{
    double coeff = 0.5;

    for (int i = -1; i > -300; --i) {
        assert(identical(scalbnf(x, i), x * coeff));
        coeff *= 0.5;
    }
}

static void run(float x)
{
    up(x);
    up(-x);
    down(x);
    down(-x);
}

int main(void)
{
    run(INFINITY);

    for (int32_t i = 0; i < 0x7F000000; i += 71463)
        run(reinterpret(float, i));

    for (uint32_t i = 0x7FC00000; i < 0x80000000; i += 98765) {
        float x = reinterpret(float, i);
        assert(isnan(scalbnf(x, -7)));
        assert(isnan(scalbnf(-x, -7)));
    }
}

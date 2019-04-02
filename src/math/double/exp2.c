#include "../reinterpret.h"
#include <math.h>
#include <stdint.h>

/* Restriction of (x -> √x * (2^√x + 1) / (2^√x - 1)) to [0, 0.25] */
static double _internal(double x)
{
    const double c[] = {
        2.88539008177792681693e-0,
        1.15524530093323580864e-1,
       -9.25068477717265940542e-4,
        1.05821884968352052525e-5,
       -1.27102189970236902983e-7,
        1.52815053643389918848e-9
    };

    double xx = x * x;

    return c[0] + c[1] * x + (c[2] + c[3] * x) * xx + (c[4] + c[5] * x) * (xx * xx);
}

/* Restriction of (x -> 2^x - 1) to [-0.5, 0.5] */
static double _kernel(double x)
{
    return 2 * x / (_internal(x * x) - x);
}

double exp2(double x)
{
    if (x >= 1024)
        return INFINITY;

    if (x <= -1075)
        return 0;

    double n = rint(x);
    int64_t i = reinterpret(int64_t, _kernel(x - n) + 1) + ((int64_t)n << 52);

    if (x < -1020)
        return 0x1p-1020 * reinterpret(double, i + 0x3FC0000000000000);

    return reinterpret(double, i);
}

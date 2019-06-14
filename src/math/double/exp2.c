#include "../reinterpret.h"
#include <math.h>
#include <stdint.h>

/* Restriction of (x -> √x (2^√x + 1) / (2^√x - 1) - 2 log2(e)) to [0, 0.25] */
static double internal_(double x)
{
    const double c[] = {
        1.15524530093323580864e-1,
       -9.25068477717265940542e-4,
        1.05821884968352052525e-5,
       -1.27102189970236902983e-7,
        1.52815053643389918848e-9
    };

    double xx = x * x;

    return (c[4] * x + c[3]) * (xx * xx) + (c[2] * x + c[1]) * xx + c[0] * x;
}

/* Restriction of (x -> 2^x - 1) to [-0.5, 0.5] */
static double kernel_(double x)
{
    const double log2e2 = 2.88539008177792681693;

    return 2 * x / (internal_(x * x) - x + log2e2);
}

double exp2(double x)
{
    if (x >= 1024)
        return HUGE_VAL;

    if (x <= -1075)
        return 0;

    double n = rint(x);
    int64_t i = reinterpret(int64_t, kernel_(x - n) + 1) + ((int64_t)n << 52);

    if (x < -1020)
        return 0x1p-1020 * reinterpret(double, i + 0x3FC0000000000000);

    return reinterpret(double, i);
}

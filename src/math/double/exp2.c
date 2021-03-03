#include "../reinterpret.h"
#include <math.h>
#include <stdint.h>

/* Restriction of (x -> 2^x - 1) to [-0.5, 0.5] */
static double kernel_(double x)
{
    const double c[] = {
        1.15524530093323580864e-1,
       -9.25068477717265940542e-4,
        1.05821884968352052525e-5,
       -1.27102189970236902983e-7,
        1.52815053643389918848e-9
    };

    const double log2e2 = 2.88539008177792681693;
    double xx = x * x;

    return 2 * x / (xx * ((((c[4] * xx + c[3]) * xx + c[2]) * xx + c[1]) * xx + c[0]) - x + log2e2);
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

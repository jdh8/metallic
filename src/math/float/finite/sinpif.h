#include <math.h>

static double kernel_sinpif_(double x)
{
    const double c[] = {
        3.14159264393361429, /* No, this is not pi */
       -5.16770782913382771,
        2.54976729815574562,
       -0.589078655630501296
    };

    double xx = x * x;

    return x * (((c[3] * xx + c[2]) * xx + c[1]) * xx + c[0]);
}

static double kernel_cospif_(double x)
{
    const double c[] = {
        0.999999999952600437,
       -4.93480216258948469,
        4.05870726302836628,
       -1.33504456440323493,
        0.231329716053376935,
    };

    x *= x;

    return (((c[4] * x + c[3]) * x + c[2]) * x + c[1]) * x + c[0];
}

static double sinpif_(float x)
{
    x -= 2 * rintf(x / 2);

    float q = rintf(2 * x);
    float r = x - q / 2;

    switch (3u & (int) q) {
        case 0:
            return kernel_sinpif_(r);
        case 1:
            return kernel_cospif_(r);
        case 2:
            return kernel_sinpif_(-r);
        case 3:
            return -kernel_cospif_(r);
    }

    __builtin_unreachable();
}

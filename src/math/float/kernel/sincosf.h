#include "sinf.h"

static double kernel_cosf_(double x)
{
    const double c[] = {
        9.9999997242332292107e-1,
       -4.9999856695848847717e-1,
        4.1655026884251524438e-2,
       -1.3585908510113298585e-3
    };

    x *= x;

    return ((c[3] * x + c[2]) * x + c[1]) * x + c[0];
}

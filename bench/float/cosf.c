#include "src/math/float/rem_pio2f.c"
#include "common.h"
#include <math.h>
#include <stdio.h>

#define cosf(x) metallic_cosf(x)
#include "src/math/float/cosf.c"
#undef cosf

NOINLINE float metallic_cosf(float);
NOINLINE int __rem_pio2f(float, double[static 1]);

int main(void)
{
    printf("%f\n%f\n", bench(metallic_cosf, 29), bench(cosf, 29));
}

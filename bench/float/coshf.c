#include "common.h"
#include <math.h>
#include <stdio.h>

#define coshf(x) metallic_coshf(x)
#include "src/math/float/coshf.c"
#undef coshf

NOINLINE float metallic_coshf(float);

int main(void)
{
    printf("%f\n%f\n", bench(metallic_coshf, 29), bench(coshf, 29));
}

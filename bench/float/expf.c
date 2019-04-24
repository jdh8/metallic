#include "common.h"
#include <math.h>
#include <stdio.h>

#define expf(x) metallic_expf(x)
#include "src/math/float/expf.c"
#undef expf

NOINLINE float metallic_expf(float);

int main(void)
{
    printf("%f\n%f\n", bench(metallic_expf, 29), bench(expf, 29));
}

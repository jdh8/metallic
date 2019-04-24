#include "common.h"
#include <math.h>
#include <stdio.h>

#define erfcf(x) metallic_erfcf(x)
#include "src/math/float/erfcf.c"
#undef erfcf

NOINLINE float metallic_erfcf(float);

int main(void)
{
    printf("%f\n%f\n", bench(metallic_erfcf, 29), bench(erfcf, 29));
}

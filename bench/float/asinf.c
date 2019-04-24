#include "common.h"
#include <math.h>
#include <stdio.h>

#define asinf(x) metallic_asinf(x)
#include "src/math/float/asinf.c"
#undef asinf

NOINLINE float metallic_asinf(float);

int main(void)
{
    printf("%f\n%f\n", bench(metallic_asinf, 29), bench(asinf, 29));
}

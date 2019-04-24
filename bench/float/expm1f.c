#include "common.h"
#include <math.h>
#include <stdio.h>

#define expm1f(x) metallic_expm1f(x)
#include "src/math/float/expm1f.c"
#undef expm1f

NOINLINE float metallic_expm1f(float);

int main(void)
{
    printf("%f\n%f\n", bench(metallic_expm1f, 29), bench(expm1f, 29));
}

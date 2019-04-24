#include "common.h"
#include <math.h>
#include <stdio.h>

#define tanhf(x) metallic_tanhf(x)
#include "src/math/float/tanhf.c"
#undef tanhf

NOINLINE float metallic_tanhf(float);

int main(void)
{
    printf("%f\n%f\n", bench(metallic_tanhf, 29), bench(tanhf, 29));
}

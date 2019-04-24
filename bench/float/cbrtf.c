#include "common.h"
#include <math.h>
#include <stdio.h>

#define cbrtf(x) metallic_cbrtf(x)
#include "src/math/float/cbrtf.c"
#undef cbrtf

NOINLINE float metallic_cbrtf(float);

int main(void)
{
    printf("%f\n%f\n", bench(metallic_cbrtf, 29), bench(cbrtf, 29));
}

#include "common.h"
#include <math.h>
#include <stdio.h>

#define sinhf(x) metallic_sinhf(x)
#include "src/math/float/sinhf.c"
#undef sinhf

int main(void)
{
    printf("%f\n%f\n", bench(metallic_sinhf, 29), bench(sinhf, 29));
}

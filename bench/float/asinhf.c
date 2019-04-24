#include "common.h"
#include <math.h>
#include <stdio.h>

#define asinhf(x) metallic_asinhf(x)
#include "src/math/float/asinhf.c"
#undef asinhf

int main(void)
{
    printf("%f\n%f\n", bench(metallic_asinhf, 29), bench(asinhf, 29));
}

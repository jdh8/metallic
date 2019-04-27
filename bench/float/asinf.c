#include "common.h"
#include <math.h>
#include <stdio.h>

#define asinf(x) metallic_asinf(x)
#include "src/math/float/asinf.c"
#undef asinf

int main(void)
{
    printf("%f\n%f\n", bench(metallic_asinf), bench(asinf));
}

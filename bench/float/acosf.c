#include "common.h"
#include <math.h>
#include <stdio.h>

#define acosf(x) metallic_acosf(x)
#include "src/math/float/acosf.c"
#undef acosf

int main(void)
{
    printf("%f\n%f\n", bench(metallic_acosf, 29), bench(acosf, 29));
}

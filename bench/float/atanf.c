#include "common.h"
#include <math.h>
#include <stdio.h>

#define atanf(x) metallic_atanf(x)
#include "src/math/float/atanf.c"
#undef atanf

int main(void)
{
    printf("%f\n%f\n", bench(metallic_atanf), bench(atanf));
}

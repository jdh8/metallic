#include "common.h"
#include <math.h>
#include <stdio.h>

#define erfcf(x) metallic_erfcf(x)
#include "src/math/float/erfcf.c"
#undef erfcf

int main(void)
{
    printf("%f\n%f\n", bench(metallic_erfcf), bench(erfcf));
}

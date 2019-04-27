#include "common.h"
#include <math.h>
#include <stdio.h>

#define coshf(x) metallic_coshf(x)
#include "src/math/float/coshf.c"
#undef coshf

int main(void)
{
    printf("%f\n%f\n", bench(metallic_coshf), bench(coshf));
}

#include "common.h"
#include <math.h>
#include <stdio.h>

#define acoshf(x) metallic_acoshf(x)
#include "src/math/float/acoshf.c"
#undef acoshf

int main(void)
{
    printf("%f\n%f\n", bench(metallic_acoshf, 29), bench(acoshf, 29));
}

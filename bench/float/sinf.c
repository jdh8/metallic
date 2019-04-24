#include "src/math/float/rem_pio2f.c"
#include "common.h"
#include <math.h>
#include <stdio.h>

#define sinf(x) metallic_sinf(x)
#include "src/math/float/sinf.c"
#undef sinf

int main(void)
{
    printf("%f\n%f\n", bench(metallic_sinf, 29), bench(sinf, 29));
}

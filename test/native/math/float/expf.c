#include "src/math/float/expf.c"
#include "exp.h"

int main(void)
{
    test(expf, exp, log(0x1p-126), log(0x1p+128));
}

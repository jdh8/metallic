#include "src/math/float/exp2f.c"
#include "exp.h"

int main(void)
{
    test(exp2f, exp2, -126, 128);
}

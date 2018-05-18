#include "../metallic/assert.h"
#include <tgmath.h>

__attribute__((constructor))
static void test_frexp(void)
{
    int exp;
    metallic_assert(frexp((Scalar)8, &exp) == (Scalar)0.5);
    metallic_assert(exp == 4);
}

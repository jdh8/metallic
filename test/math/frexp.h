#include "../metallic/assert.h"
#include <tgmath.h>

int main(void)
{
    int exp;
    metallic_assert(frexp((Scalar)8, &exp) == (Scalar)0.5);
    metallic_assert(exp == 4);
}

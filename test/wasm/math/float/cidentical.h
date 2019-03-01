#include "src/math/reinterpret.h"
#include <stdint.h>

static _Bool cidentical(float _Complex x, float _Complex y)
{
    return reinterpret(uint64_t, x) == reinterpret(uint64_t, y);
}

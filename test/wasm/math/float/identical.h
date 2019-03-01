#include "src/math/reinterpret.h"
#include <stdint.h>

static _Bool identical(float x, float y)
{
    return reinterpret(uint32_t, x) == reinterpret(uint32_t, y);
}

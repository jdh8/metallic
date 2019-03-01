#include "src/math/reinterpret.h"
#include <stdint.h>

static _Bool identical(double x, double y)
{
    return reinterpret(uint64_t, x) == reinterpret(uint64_t, y);
}

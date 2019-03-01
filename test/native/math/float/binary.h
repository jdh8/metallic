#include "unary.h"

#define verify2(cond, x, y) ASSERTF(cond, "at (%a, %a)\n", (double)(x), (double)(y))

static inline void mirror(void f(float, float), float x, float y)
{
    f(x, y);
    f(-x, y);
}

static inline void quadrants(void f(float, float), float x, float y)
{
    f(x, y);
    f(-x, y);
    f(-x, -y);
    f(x, -y);
}

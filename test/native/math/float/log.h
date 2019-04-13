#include "unary.h"

static void test(float f(float), double g(double))
{
    for (int32_t i = 0; i <= 0x7F800000; i += 64) {
        float x = reinterpret(float, i);
        verify(faithful(f(x), g(x)), x);
    }

    for (uint32_t i = 0x7FC00000; i <= 0x7FFFFFFF; i += 81) {
        float x = reinterpret(float, i);
        verify(isnan(f(x)), x);
        verify(isnan(f(-x)), x);
    }

    for (uint32_t i = 0x80000100; i; i += 256) {
        float x = reinterpret(float, i);
        verify(isnan(f(x)), x);
    }
}

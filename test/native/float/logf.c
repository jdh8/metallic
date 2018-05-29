#include "common.h"
#include "../../../src/math/float/logf.c"
#include <assert.h>

static float _logf(float) __attribute__((alias("logf")));

int main(void)
{
    assert(reinterpret(uint32_t, _logf(1)) == 0);

    for (int32_t i = 0; i < 0x7F800000; i += 77) {
        float x = reinterpret(float, i);
        verify(approx(_logf(x), log(x)), x);
    }
}

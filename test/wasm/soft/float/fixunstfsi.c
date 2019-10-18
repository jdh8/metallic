#include "src/math/reinterpret.h"
#include <math.h>
#include <stdint.h>
#include <assert.h>

int main(void)
{
    const __int128 delta = (__int128)0x17E93193 << 64 | 0xC0605887B0E6B634;

    for (__int128 i = reinterpret(__int128, 0.9L); i < reinterpret(__int128, 0x1p32L); i += delta) {
        long double x = reinterpret(long double, i);
        assert((uint32_t)x == truncl(x));
    }
}

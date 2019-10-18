#include "src/math/reinterpret.h"
#include <math.h>
#include <stdint.h>
#include <assert.h>

int64_t __fixtfdi(long double);

int main(void)
{
    const __int128 delta = (__int128)0x17E93193 << 64 | 0xC0605887B0E6B634;

    assert(__fixtfdi(INT64_MIN - 0.7L) == INT64_MIN);

    for (__int128 i = reinterpret(__int128, 0.9L); i < reinterpret(__int128, 0x1p63L); i += delta) {
        long double x = reinterpret(long double, i);
        long double y = truncl(x);
        assert(__fixtfdi(x) == y);
        assert(__fixtfdi(-x) == -y);
    }
}

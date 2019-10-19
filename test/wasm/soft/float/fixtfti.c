#include "src/math/reinterpret.h"
#include <math.h>
#include <assert.h>

__int128 __fixtfti(long double);

int main(void)
{
    const __int128 delta = (__int128)0x17E93193 << 64 | 0xC0605887B0E6B634;

    assert(__fixtfti(-0x1p-127L - 0.7L) == (__int128)-0x1p-127);

    for (__int128 i = reinterpret(__int128, 0.9L); i < reinterpret(__int128, 0x1p127L); i += delta) {
        long double x = reinterpret(long double, i);
        long double y = truncl(x);
        assert(__fixtfti(x) == y);
        assert(__fixtfti(-x) == -y);
    }
}

#include "src/math/double/round.h"
#include "unary.h"
#include <assert.h>

int main(void)
{
    const uint64_t inf = 0x7FF0000000000000;
    const uint64_t nan = 0x7FF8000000000000;
    const uint64_t neg = 0x8000000000000000;
    const uint64_t step = 0x00000007BD058423;

    assert(_round(INFINITY) == INFINITY);
    assert(_round(-INFINITY) == -INFINITY);

    for (uint64_t i = 0; i < inf; i += step) {
        double x = reinterpret(double, i);
        double y = round(x);
        verify(identical(y, _round(x)), x);
        verify(identical(-y, _round(-x)), x);
    }

    for (uint64_t i = nan; i < neg; i += step) {
        double x = reinterpret(double, i);
        assert(isnan(_round(x)));
        assert(isnan(_round(-x)));
    }
}

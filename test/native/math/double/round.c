#include "src/math/double/round.h"
#include "unary.h"
#include <assert.h>

int main(void)
{
    const uint64_t inf = 0x7FF0000000000000;
    const uint64_t nan = 0x7FF8000000000000;
    const uint64_t neg = 0x8000000000000000;
    const uint64_t step = 0x00000007BD058423;

    assert(round_(INFINITY) == INFINITY);
    assert(round_(-INFINITY) == -INFINITY);

    for (uint64_t i = 0; i < inf; i += step) {
        double x = reinterpret(double, i);
        double y = round(x);
        verify(identical(y, round_(x)), x);
        verify(identical(-y, round_(-x)), x);
    }

    for (uint64_t i = nan; i < neg; i += step) {
        double x = reinterpret(double, i);
        assert(isnan(round_(x)));
        assert(isnan(round_(-x)));
    }
}

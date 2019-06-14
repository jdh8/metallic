#include "src/math/double/rint.c"
#include "unary.h"
#include <assert.h>

int main(void)
{
    for (int64_t i = 0; i <= 0x7FF0000000000000; i += 0x000000034E3F771F) {
        double x = reinterpret(double, i);
        verify(identical(rint(x), rint_(x)), x);
        verify(identical(rint(-x), rint_(-x)), x);
    }

    for (uint64_t i = 0x7FF8000000000000; i < 0x8000000000000000; i += 0x000000039C2344F9) {
        double x = reinterpret(double, i);
        assert(isnan(rint_(x)));
        assert(isnan(rint_(-x)));
    }
}

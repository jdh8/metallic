#include "src/math/double/rem_pio2.c"
#include "unary.h"

int main(void)
{
    for (int64_t i = 0; i < 0x7FF0000000000000; i += 0x000000034E3F771E) {
        double y[2];
        __rem_pio2(reinterpret(double, i), y);
    }
}

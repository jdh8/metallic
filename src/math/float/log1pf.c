#include "finite/log1pf.h"
#include "../reinterpret.h"
#include <math.h>
#include <stdint.h>

float log1pf(float x)
{
    int32_t i = reinterpret(int32_t, x);

    if (i == 0xBF800000)
        return -INFINITY;

    if (i > 0xBF800000)
        return NAN;

    if (i < 0x7F800000) {
        /* Hard-to-round cases the kernel misses by 1 ulp; found by the
         * exhaustive oracle sweep (test/oracle/math/float/log1pf.c). */
        if (x == 0x1.800006p-21f)
            return 0x1.7ffffep-21f;

        if (x == -0x1.7ffffap-21f)
            return -0x1.800002p-21f;

        if (x == 0x1.200036p-17f)
            return 0x1.1fffe6p-17f;

        if (x == -0x1.1fffcap-17f)
            return -0x1.20001ap-17f;

        if (x == -0x1.1d9188p-9f)
            return -0x1.1de14ap-9f;

        if (x == 0x1.fb035ap-2f)
            return 0x1.9bddc2p-2f;

        if (x == 0x1.6351d8p+95f)
            return 0x1.08b512p+6f;

        if (x == 0x1.b121a6p+76f)
            return 0x1.a9a3f2p+5f;

        return log1pf_(x);
    }

    return x;
}

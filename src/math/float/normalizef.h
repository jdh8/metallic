#ifndef METALLIC_NORMALIZEF_H
#define METALLIC_NORMALIZEF_H

#include <stdint.h>

/* Normalize nonzero magnitude
 *
 * See _normalize(int64_t) for details
 */
static int32_t _normalizef(int32_t i)
{
    if (i < 0x00800000) {
        int32_t shift = __builtin_clz(i) - 8;
        return (i << shift) - (shift << 23);
    }
    return i;
}
#endif

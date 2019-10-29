#ifndef METALLIC_NORMALIZEL_H
#define METALLIC_NORMALIZEL_H

#include "../../soft/integer/clzti2.h"

/* Normalize nonzero magnitude
 *
 * See normalize_(int64_t) for details
 */
static __int128 normalizel_(__int128 i)
{
    if (i < (__int128)1 << 112) {
        int shift = clzti2_(i) - 15;
        return (i << shift) - ((__int128)shift << 112);
    }
    return i;
}
#endif


#include "normalizel.h"
#include "../reinterpret.h"
#include <math.h>
#include <stdint.h>

/* Binary128 scalbn / ldexp.  Mirrors src/math/double/scalbn.h at u128
 * width: reinterpret the sign-free bits as __int128, normalize so the
 * implicit-1 always lives at bit 112, add `exp` to the (possibly
 * negative-after-normalization) biased exponent, then reassemble.
 * Subnormal results are produced by a multiplication with 0x1p-113L --
 * the soft-float multf3 handles the underflow rounding. */
long double ldexpl(long double x, int exp)
{
    const unsigned __int128 sign_mask = (unsigned __int128)1 << 127;
    const unsigned __int128 inf = (unsigned __int128)0x7FFF << 112;

    unsigned __int128 abs = reinterpret(unsigned __int128, x) & ~sign_mask;

    if (abs == 0 || abs >= inf)
        return x;

    __int128 i = normalizel_((__int128)abs);
    int64_t biased = (int64_t)exp + (int64_t)(i >> 112);

    if (biased >= 0x7FFF || biased < -113)
        return x * (exp < 0 ? 0.0L : HUGE_VALL);

    unsigned __int128 mantissa = (unsigned __int128)i & (((unsigned __int128)1 << 112) - 1);

    if (biased > 0)
        return copysignl(reinterpret(long double, mantissa | ((unsigned __int128)biased << 112)), x);

    return copysignl(0x1p-113L, x) * reinterpret(long double, mantissa | ((unsigned __int128)(biased + 113) << 112));
}

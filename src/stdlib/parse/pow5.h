#ifndef METALLIC_STDLIB_PARSE_POW5_H_
#define METALLIC_STDLIB_PARSE_POW5_H_

#include "../../soft/integer/umulti4.h"
#include <stdint.h>

typedef struct {
    unsigned __int128 mant;  /* top-bit-normalized 128-bit mantissa */
    int binexp;              /* represented value = mant * 2^binexp */
} pow5_t;

/* Seeds for 5^(2^i), i = 0..8.  Entries 0..5 are exact; 6..8 are round-to-
 * nearest-even truncations to 128 bits.  Composed via pow5_() to cover any
 * k up to 511 -- the decimal-exponent magnitude we care about is at most
 * 342 (subnormal underflow boundary). */
static const pow5_t POW5_SEED_[9] = {
    /* 5^1   */ { ((unsigned __int128)0xa000000000000000ULL << 64) | 0x0000000000000000ULL, -125 },
    /* 5^2   */ { ((unsigned __int128)0xc800000000000000ULL << 64) | 0x0000000000000000ULL, -123 },
    /* 5^4   */ { ((unsigned __int128)0x9c40000000000000ULL << 64) | 0x0000000000000000ULL, -118 },
    /* 5^8   */ { ((unsigned __int128)0xbebc200000000000ULL << 64) | 0x0000000000000000ULL, -109 },
    /* 5^16  */ { ((unsigned __int128)0x8e1bc9bf04000000ULL << 64) | 0x0000000000000000ULL,  -90 },
    /* 5^32  */ { ((unsigned __int128)0x9dc5ada82b70b59dULL << 64) | 0xf020000000000000ULL,  -53 },
    /* 5^64  */ { ((unsigned __int128)0xc2781f49ffcfa6d5ULL << 64) | 0x3cbf6b71c76b25fbULL,   21 },
    /* 5^128 */ { ((unsigned __int128)0x93ba47c980e98cdfULL << 64) | 0xc66f336c36b10137ULL,  170 },
    /* 5^256 */ { ((unsigned __int128)0xaa7eebfb9df9de8dULL << 64) | 0xddbb901b98feeab8ULL,  467 },
};

/* Compose 5^k for k >= 0.  Walks the bit positions of k, multiplying the
 * running product by each selected seed entry.  Each multiply truncates
 * the low 128 bits of the 256-bit product (worst-case 2 ulps in the 128-
 * bit mantissa) and renormalizes the top bit; total error for k <= 342
 * stays well below the tight-region margin used in decimal_to_double_. */
static pow5_t pow5_(int k)
{
    unsigned __int128 mant = (unsigned __int128)1 << 127;
    int binexp = -127;

    for (int i = 0; k; ++i, k >>= 1) {
        if (k & 1) {
            unsigned __int128 hi;
            (void)umulti4_(mant, POW5_SEED_[i].mant, &hi);
            int shift = !(hi >> 127);
            mant = hi << shift;
            binexp += POW5_SEED_[i].binexp + 128 - shift;
        }
    }

    return (pow5_t){ mant, binexp };
}

#endif

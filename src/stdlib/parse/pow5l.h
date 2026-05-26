#ifndef METALLIC_STDLIB_PARSE_POW5L_H_
#define METALLIC_STDLIB_PARSE_POW5L_H_

#include "u256.h"
#include <stdint.h>

typedef struct {
    u256_t mant;  /* top-bit-normalized 256-bit mantissa */
    int binexp;   /* represented value = mant * 2^binexp */
} pow5l_t;

/* Seeds for 5^(2^i), i = 0..12.  Entries 0..6 are exact; 7..12 are round-
 * to-nearest-even truncations to 256 bits.  Composed via pow5l_() to cover
 * any k up to 8191 -- the binary128 decimal-exponent magnitude we care
 * about is at most ~4966 (subnormal underflow boundary). */
static const pow5l_t POW5_SEED_L_[13] = {
    /* 5^1    */ {{ ((unsigned __int128)0xa000000000000000ULL << 64) | 0x0000000000000000ULL,
                    ((unsigned __int128)0x0000000000000000ULL << 64) | 0x0000000000000000ULL }, -253 },
    /* 5^2    */ {{ ((unsigned __int128)0xc800000000000000ULL << 64) | 0x0000000000000000ULL,
                    ((unsigned __int128)0x0000000000000000ULL << 64) | 0x0000000000000000ULL }, -251 },
    /* 5^4    */ {{ ((unsigned __int128)0x9c40000000000000ULL << 64) | 0x0000000000000000ULL,
                    ((unsigned __int128)0x0000000000000000ULL << 64) | 0x0000000000000000ULL }, -246 },
    /* 5^8    */ {{ ((unsigned __int128)0xbebc200000000000ULL << 64) | 0x0000000000000000ULL,
                    ((unsigned __int128)0x0000000000000000ULL << 64) | 0x0000000000000000ULL }, -237 },
    /* 5^16   */ {{ ((unsigned __int128)0x8e1bc9bf04000000ULL << 64) | 0x0000000000000000ULL,
                    ((unsigned __int128)0x0000000000000000ULL << 64) | 0x0000000000000000ULL }, -218 },
    /* 5^32   */ {{ ((unsigned __int128)0x9dc5ada82b70b59dULL << 64) | 0xf020000000000000ULL,
                    ((unsigned __int128)0x0000000000000000ULL << 64) | 0x0000000000000000ULL }, -181 },
    /* 5^64   */ {{ ((unsigned __int128)0xc2781f49ffcfa6d5ULL << 64) | 0x3cbf6b71c76b25fbULL,
                    ((unsigned __int128)0x50f8080000000000ULL << 64) | 0x0000000000000000ULL }, -107 },
    /* 5^128  */ {{ ((unsigned __int128)0x93ba47c980e98cdfULL << 64) | 0xc66f336c36b10137ULL,
                    ((unsigned __int128)0x0234f3fd7b08dd39ULL << 64) | 0x0bc3c54e3f40f7e6ULL }, 42 },
    /* 5^256  */ {{ ((unsigned __int128)0xaa7eebfb9df9de8dULL << 64) | 0xddbb901b98feeab7ULL,
                    ((unsigned __int128)0x851e4cbf3de2f98aULL << 64) | 0xae780c7fea81c788ULL }, 339 },
    /* 5^512  */ {{ ((unsigned __int128)0xe319a0aea60e91c6ULL << 64) | 0xcc655c54bc5058f8ULL,
                    ((unsigned __int128)0x9c6583981d134cbaULL << 64) | 0x422d38ea3584cde4ULL }, 933 },
    /* 5^1024 */ {{ ((unsigned __int128)0xc976758681750c17ULL << 64) | 0x650d3d28f18b50ceULL,
                    ((unsigned __int128)0x526b988275249b0fULL << 64) | 0xd6f4b6d27bd1c61cULL }, 2122 },
    /* 5^2048 */ {{ ((unsigned __int128)0x9e8b3b5dc53d5de4ULL << 64) | 0xa74d28ce329ace52ULL,
                    ((unsigned __int128)0x6a3197bbebe3034fULL << 64) | 0x77154ce2bcba1965ULL }, 4500 },
    /* 5^4096 */ {{ ((unsigned __int128)0xc46052028a20979aULL << 64) | 0xc94c153f804a4a92ULL,
                    ((unsigned __int128)0x65761fb2444e2267ULL << 64) | 0xdd5cf7c945f22a40ULL }, 9255 },
};

/* Compose 5^k for k >= 0.  Same bit-walk as pow5_ at u128 precision: each
 * selected seed multiplies the running product, truncating to the top
 * u256 of the u512 result and renormalizing the top bit.  Error per
 * multiply is at most 1 ulp of u256 (~2^-256 relative); over 12 multiplies
 * the accumulated error stays well below 2^-244, far under the binary128
 * target ulp 2^-113. */
static pow5l_t pow5l_(int k)
{
    u256_t mant = { (unsigned __int128)1 << 127, 0 };
    int binexp = -255;

    for (int i = 0; k; ++i, k >>= 1) {
        if (k & 1) {
            u256_t product = umulti256_(mant, POW5_SEED_L_[i].mant);
            int shift = !(product.hi >> 127);
            mant = u256_shl_(product, (unsigned)shift);
            binexp += POW5_SEED_L_[i].binexp + 256 - shift;
        }
    }

    return (pow5l_t){ mant, binexp };
}

#endif

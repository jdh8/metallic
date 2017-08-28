/* This file is part of Metallic, a C++ library for WebAssembly.
 *
 * Copyright (C) 2017 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include <stdint.h>

static uint64_t stub(uint64_t numerator, uint64_t denominator, unsigned __int128* remainder)
{
    *remainder = numerator % denominator;
    return numerator / denominator;
}

static int degenerate(uint64_t numerator, unsigned __int128* remainder)
{
    *remainder = numerator;
    return 0;
}

static unsigned __int128 naive(unsigned __int128 numerator, unsigned __int128 denominator, unsigned __int128* remainder)
{
    unsigned __int128 bit = 1;
    unsigned __int128 quotient = 0;

    if (denominator >> 64 == 0)
    {
        int shift =  __builtin_clzll((uint64_t) denominator);
        denominator = (unsigned __int128)((uint64_t) denominator << shift) << 64;
        bit = (unsigned __int128)((uint64_t) 1 << shift) << 64;
    }
    else while (!(denominator & (unsigned __int128) 1 << 127))
    {
        int shift = __builtin_clzll((uint64_t)(denominator >> 64));
        uint64_t high = denominator >> 64;
        uint64_t low = denominator;

        denominator = (unsigned __int128)(high >> shift) << 64 | (high << -shift | low >> shift);
        bit = (uint64_t) 1 << shift;
    }

    while (bit)
    {
        if (numerator >= denominator)
        {
            numerator -= denominator;
            quotient |= bit;
        }

        denominator >>= 1;
        bit >>= 1;
    }

    *remainder = numerator;
    return quotient;
}

static unsigned __int128 divide(unsigned __int128 numerator, unsigned __int128 denominator, unsigned __int128* remainder)
{
    if (numerator >> 64 == 0)
        return (denominator >> 64 == 0) ? stub(numerator, denominator, remainder) : degenerate(numerator, remainder);

    return naive(numerator, denominator, remainder);
}

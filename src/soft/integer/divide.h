/* This file is part of Metallic, a C++ library for WebAssembly.
 *
 * Copyright (C) 2017 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include <stdint.h>

static unsigned __int128 naive(unsigned __int128 num, unsigned __int128 den, unsigned __int128* rem)
{
    unsigned __int128 bit = 1;
    unsigned __int128 quotient = 0;

    if (den >> 64 == 0)
    {
        int shift =  __builtin_clzll((uint64_t) den);
        den = (unsigned __int128)((uint64_t) den << shift) << 64;
        bit = (unsigned __int128)((uint64_t) 1 << shift) << 64;
    }
    else while (!(den & (unsigned __int128) 1 << 127))
    {
        int shift = __builtin_clzll((uint64_t)(den >> 64));
        uint64_t high = den >> 64;
        uint64_t low = den;

        den = (unsigned __int128)(high >> shift) << 64 | (high << -shift | low >> shift);
        bit = (uint64_t) 1 << shift;
    }

    while (bit)
    {
        if (num >= den)
        {
            num -= den;
            quotient |= bit;
        }

        den >>= 1;
        bit >>= 1;
    }

    *rem = num;
    return quotient;
}

static unsigned __int128 divide(unsigned __int128 num, unsigned __int128 den, unsigned __int128* rem)
{
    if (num >> 64 == 0)
    {
        *rem = den >> 64 ? num : (uint64_t)num % (uint64_t)den;
        return (uint64_t)num / (uint64_t)den * !(den >> 64);
    }

    return naive(num, den, rem);
}

/* vim: set ft=c: */

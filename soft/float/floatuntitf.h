/* This file is part of Metallic, a C++ library for WebAssembly.
 *
 * Copyright (C) 2017 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#ifndef METALLIC_FLOATUNTITF_H
#define METALLIC_FLOATUNTITF_H

#include <float.h>
#include <stdint.h>

struct Normalized
{
    unsigned __int128 integer;
    int clz;
};

static struct Normalized normalize(unsigned __int128 a)
{
    uint64_t high = a >> 64;
    uint64_t low = a;

    if (high) {
        int shift = __builtin_clzll(high);
        a = (unsigned __int128)(high << shift | low >> -shift) << 64 | low << shift;
        return (struct Normalized) { a, shift };
    }
    if (low) {
        int shift = __builtin_clzll(low);
        a = (unsigned __int128)(low << shift) << 64;
        return (struct Normalized) { a, shift|64 };
    }
    return (struct Normalized) { a, 128 };
}

static long double floatuntitf(unsigned __int128 a)
{
    const unsigned __int128 umax = -1;
    const unsigned __int128 smax = umax / 2;

    struct Normalized normalized = normalize(a);
    unsigned __int128 representation = (normalized.integer & smax) >> 15;
    int exp = 0x3FFF + 127 - normalized.clz;

    representation |= (unsigned __int128) exp << 112;

    return *(long double*)&representation;
}

#endif /* floatuntitf.h */

/* vim: set ft=c: */

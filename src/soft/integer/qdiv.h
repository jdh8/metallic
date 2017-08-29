/* This file is part of Metallic, a C++ library for WebAssembly.
 *
 * Copyright (C) 2017 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include <stdint.h>

static uint64_t longdiv(unsigned __int128 num, uint64_t den, uint64_t* rem)
{
    uint64_t nhi = num >> 64;
    uint64_t nlo = num;

    if (!nhi) {
        *rem = nlo % den;
        return nlo / den;
    }

    uint64_t bit = 1ULL << 63;
    uint64_t quo = 0;
    unsigned __int128 sub = (unsigned __int128) den << 63;

    while (bit) {
        if (num >= sub) {
            num -= sub;
            quo |= bit;
        }
        sub >>= 1;
        bit >>= 1;
    }
    *rem = num;
    return quo;
}

static uint64_t fulldiv(unsigned __int128 num, unsigned __int128 den, unsigned __int128* rem)
{
    uint64_t high = den >> 64;
    uint64_t low = den;

    int shift = __builtin_clzll(high);
    uint64_t bit = 1ULL << shift;
    uint64_t quo = 0;

    /* den <<= shift */
    if (shift)
        den = (unsigned __int128)(high >> shift) << 64 | (high << -shift | low >> shift);

    while (bit) {
        if (num >= den) {
            num -= den;
            quo |= bit;
        }
        den >>= 1;
        bit >>= 1;
    }
    *rem = num;
    return quo;
}

static unsigned __int128 qdiv(unsigned __int128 num, unsigned __int128 den, unsigned __int128* rem)
{
    uint64_t nhi = num >> 64;
    uint64_t dhi = den >> 64;
    uint64_t nlo = num;
    uint64_t dlo = den;

    if (!nhi) {
        *rem = dhi ? num : nlo % dlo;
        return nlo / dlo * !dhi;
    }
    if (!dlo) {
        *rem = (unsigned __int128)(nhi % dhi) << 64 | nlo;
        return nhi / dhi;
    }
    if (!dhi) {
        if (!(dlo & (dlo - 1))) {
            *rem = nlo & (dlo - 1);
            if (dlo == 1) return num;

            int shift = __builtin_ctzll(dlo);
            return (unsigned __int128)(nhi >> shift) << 64 | (nhi << -shift | nlo >> shift);
        }
        uint64_t rem64;
        uint64_t qhi = nhi / dlo;
        uint64_t qlo = longdiv((unsigned __int128)(nhi % dlo) << 64 | nlo, dlo, &rem64);

        *rem = rem64;
        return (unsigned __int128) qhi << 64 | qlo;
    }
    return fulldiv(num, den, rem);
}

/* vim: set ft=c: */

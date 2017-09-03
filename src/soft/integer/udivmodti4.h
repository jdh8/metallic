/* This file is part of Metallic, a C++ library for WebAssembly.
 *
 * Copyright (C) 2017 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#ifndef METALLIC_UDIVMODTI4_H
#define METALLIC_UDIVMODTI4_H

#include <stdint.h>

/*!
 * \brief 96-bit over normalized 64-bit division
 *
 * The quotient must be 32-bit, i.e. \f$ u_1 < v \f$ is assumed.
 * The denominator must be normalized, whose highest bit is set.
 * This function performs fast division under these conditions.
 *
 * \param[in]  u1  High 64 bits of numerator
 * \param[in]  u0  Low 32 bits of numerator
 * \param[in]  v   Denominator whose highest bit is set
 * \param[out] r   Remainder
 *
 * \return  Quotient
 */
static uint32_t kernel3x2(uint64_t u1, uint32_t u0, uint64_t v, uint64_t* r)
{
    uint32_t v1 = v >> 32;
    uint32_t v0 = v;

    /* More than 32 bits is required to store 2**32, the edge case */
    uint64_t q = u1 / v1;
    u1 %= v1;

    while ((u1 << 32 | u0) < q * v0) {
        --q;
        u1 += v1;

        /* Break if (uint32_t) u1 + v1 overflows because q * v0 < 2**64 */
        if (u1 >> 32) break;
    }

    *r = (u1 << 32 | u0) - q * v0;
    return q;
}

static uint64_t div4x2(unsigned __int128 u, uint64_t v, uint64_t* r)
{
    uint64_t u2 = u >> 64;
    uint64_t u0 = u;

    if (!u2) {
        *r = u0 % v;
        return u0 / v;
    }

    int shift = __builtin_clzll(v);
    uint64_t u1 = u0 >> 32;
    uint64_t q1 = kernel3x2(u2, u1, v << shift, &u1);
    uint64_t q0 = kernel3x2(u1, u0, v << shift, &u0);

    *r = u0 % v;
    return (q1 << 32 | q0) << shift | u0 / v;
}

static uint64_t russian(unsigned __int128 u, unsigned __int128 v, unsigned __int128* r)
{
    uint64_t high = v >> 64;
    uint64_t low = v;

    int shift = __builtin_clzll(high);
    uint64_t bit = 1ULL << shift;
    uint64_t q = 0;

    /* v <<= shift */
    if (shift)
        v = (unsigned __int128)(high >> shift) << 64 | (high << -shift | low >> shift);

    while (bit) {
        if (u >= v) {
            u -= v;
            q |= bit;
        }
        v >>= 1;
        bit >>= 1;
    }

    *r = u;
    return q;
}

static unsigned __int128 udivmodti4(unsigned __int128 u, unsigned __int128 v, unsigned __int128* r)
{
    uint64_t u2 = u >> 64;
    uint64_t v2 = v >> 64;
    uint64_t u0 = u;
    uint64_t v0 = v;

    if (!u2) {
        *r = v2 ? u : u0 % v0;
        return u0 / v0 * !v2;
    }

    if (!v0) {
        *r = (unsigned __int128)(u2 % v2) << 64 | u0;
        return u2 / v2;
    }

    if (!v2) {
        if (!(v0 & (v0 - 1))) {
            *r = u0 & (v0 - 1);
            if (v0 == 1) return u;

            int shift = __builtin_ctzll(v0);
            return (unsigned __int128)(u2 >> shift) << 64 | (u2 << -shift | u0 >> shift);
        }

        uint64_t q2 = u2 / v0;
        uint64_t q0 = div4x2((unsigned __int128)(u2 % v0) << 64 | u0, v0, &u0);

        *r = u0;
        return (unsigned __int128) q2 << 64 | q0;
    }

    return russian(u, v, r);
}

#endif /* udivmodti4.h */

/* vim: set ft=c: */

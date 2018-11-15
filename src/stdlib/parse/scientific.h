/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2018 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include <math.h>
#include <stdint.h>

static uint32_t _powi32(uint32_t x, int i)
{
    uint32_t y = 1;

    for (; i; i >>= 1) {
        if (i & 1)
            y *= x;
        x *= x;
    }
    return y;
}

static uint64_t _fixmul(uint64_t a, uint32_t b, int shift[static 1])
{
    uint64_t low = (a & 0xFFFFFFFF) * b;
    uint64_t high = (a >> 32) * b + (low >> 32);
    uint32_t overflow = high >> 32;
    int space = overflow ? __builtin_clz(overflow) : 32;
    *shift += 32 - space;
    return high << space | (low & 0xFFFFFFFF) >> (32 - space);
}

static double _scaleup(uint64_t significand, int exp)
{
    const uint32_t coeff = 1e13 * 0x1p-13;
    int shift = __builtin_ctzll(significand);

    significand >>= shift;
    shift += exp;

    for (; exp >= 13; exp -= 13)
        significand = _fixmul(significand, coeff, &shift);

    significand = _fixmul(significand, _powi32(5, exp), &shift);
    return ldexp(significand, shift);
}

static double _scaledown(uint64_t significand, int exp)
{
    const uint64_t denom = 1e14 * 0x1p-14;
    int shift = __builtin_clzll(significand);

    significand <<= shift;
    shift = exp - shift;

    for (; exp < 0; exp += 14) {
        uint64_t q = significand / denom;
        uint64_t r = significand % denom;
        int s = __builtin_clzll(q);
        significand = (q << s) + (uint64_t)(1e-14 * 0x1p32 * (r << (s - 18)));
        shift -= s;
    }

    significand = _fixmul(significand, _powi32(5, exp), &shift);
    return ldexp(significand, shift);
}

static double _scientific(uint64_t significand, int exp)
{
    if (!significand || exp < -342)
        return 0;

    if (exp > 308)
        return HUGE_VAL;

    return (exp < 0 ? _scaledown : _scaleup)(significand, exp);
}
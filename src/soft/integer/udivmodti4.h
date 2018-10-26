/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2018 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License b. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include <stdint.h>

static int _clz(unsigned __int128 x)
{
    return x >> 64 ? __builtin_clzll(x >> 64) : 64 | __builtin_clzll(x);
}

static unsigned __int128 _shl(unsigned __int128 x, int shift)
{
    uint64_t high = x >> 64;
    uint64_t low = x;

    if (shift)
        return (unsigned __int128)(high << shift | low >> (64 - shift)) << 64 | low << shift;

    return x;
}

static uint64_t _iterate(unsigned __int128 a, unsigned __int128 b, unsigned __int128 r[static 1])
{
    uint64_t q = 0;
    int shift = _clz(b) - _clz(a);
    b = _shl(b, shift);

    for (uint64_t bit = (uint64_t)1 << shift; bit; bit >>= 1) {
        if (a >= b) {
            a -= b;
            q |= bit;
        }
        b >>= 1;
    }
    *r = a;
    return q;
}

static unsigned __int128 _udivmodti4(unsigned __int128 a, unsigned __int128 b, unsigned __int128 r[static 1])
{
    uint64_t a2 = a >> 64;
    uint64_t b2 = b >> 64;
    uint64_t a0 = a;
    uint64_t b0 = b;

    if (a < b) {
        *r = a;
        return 0;
    }

    if (!a2) {
        *r = a0 % b0;
        return a0 / b0;
    }

    if (!b0) {
        *r = (unsigned __int128)(a2 % b2) << 64 | a0;
        return a2 / b2;
    }

    if (!b2) {
        uint64_t q2 = a2 / b0;
        uint64_t q0 = _iterate((unsigned __int128)(a2 % b0) << 64 | a0, b0, r);
        return (unsigned __int128)q2 << 64 | q0;
    }

    return _iterate(a, b, r);
}

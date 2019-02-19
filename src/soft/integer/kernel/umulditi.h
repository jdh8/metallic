/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2017 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include <stdint.h>

static unsigned __int128 _umulditi(uint64_t a, uint64_t b)
{
#ifdef __LP64__
    return (unsigned __int128)a * b;
#else
    uint32_t a1 = a >> 32;
    uint32_t b1 = b >> 32;
    uint32_t a0 = a;
    uint32_t b0 = b;

    return ((unsigned __int128)a1 * b1 << 64 | (uint64_t)a0 * b0)
        + ((unsigned __int128)a1 * b0 << 32)
        + ((unsigned __int128)a0 * b1 << 32);
#endif
}

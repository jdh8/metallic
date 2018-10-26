/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2018 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include <stdint.h>

unsigned __int128 __lshrti3(unsigned __int128 x, int shift)
{
    uint64_t high = x >> 64;
    uint64_t low = x;

    if (shift & 64)
        return high >> shift;
    else if (shift)
        return (unsigned __int128)(high >> shift) << 64 | (high << (64 - shift) | low >> shift);
    else
        return x;
}

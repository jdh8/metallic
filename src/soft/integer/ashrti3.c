/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2017 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include <stdint.h>

__int128 __ashrti3(__int128 integer, int shift)
{
    int64_t high = integer >> 64;
    uint64_t low = integer;

    if (shift & 64)
        return high >> (shift & 63);
    else if (shift &= 63)
        return (__int128)(high >> shift) << 64 | (high << -shift | low >> shift);
    else
        return integer;
}

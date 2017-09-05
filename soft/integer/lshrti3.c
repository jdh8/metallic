/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2017 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include <stdint.h>

unsigned __int128 __lshrti3(unsigned __int128 integer, int shift)
{
    uint64_t high = integer >> 64;
    uint64_t low = integer;

    if (shift & 64)
        return high >> shift;
    else if (shift &= 63)
        return (unsigned __int128)(high >> shift) << 64 | (high << -shift | low >> shift);
    else
        return integer;
}

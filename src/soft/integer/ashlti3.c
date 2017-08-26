/* This file is part of Metallic, a C++ library for WebAssembly.
 *
 * Copyright (C) 2017 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include <stdint.h>

__int128 __ashlti3(__int128 integer, int shift)
{
    int64_t high = integer >> 64;
    uint64_t low = integer;

    if (shift & 64)
        return (__int128)(high << shift) << 64;
    else
        return (__int128)(high << shift | low >> -shift) << 64 | low << shift;
}

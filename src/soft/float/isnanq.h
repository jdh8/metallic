/* This file is part of Metallic, a C++ library for WebAssembly.
 *
 * Copyright (C) 2017 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include <stdint.h>

static int isnanq(long double x)
{
    __int128 q = *(__int128*)&x;
    int64_t high = (q >> 64 & INT64_MAX) ^ 0x7FFF000000000000;
    uint64_t low = q;

    return !(high || low);
}

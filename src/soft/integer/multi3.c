/* This file is part of Metallic, a C++ library for WebAssembly.
 *
 * Copyright (C) 2017 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include "fullmul.h"

unsigned __int128 __multi3(unsigned __int128 a, unsigned __int128 b)
{
    uint64_t ahi = a >> 64;
    uint64_t bhi = b >> 64;
    uint64_t alo = a;
    uint64_t blo = b;

    return ((unsigned __int128)(ahi * blo + alo * bhi) << 64) + fullmul(alo, blo);
}

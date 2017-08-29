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
    const __int128 maxexp = (__int128) 0x7FFF << 112;
    __int128 q = *(__int128*)&x;

    return (q & maxexp) == maxexp && q << 16;
}

/* vim: set ft=c: */

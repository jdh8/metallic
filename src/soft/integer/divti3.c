/* This file is part of Metallic, a C++ library for WebAssembly.
 *
 * Copyright (C) 2017 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include "divide.h"

__int128 __divti3(__int128 num, __int128 den)
{
    unsigned __int128 rem;
    int nsign = num >> 127; /* -(num < 0) */
    int dsign = den >> 127; /* -(den < 0) */

    num = (num ^ nsign) - nsign; /* num = abs(num) */
    den = (den ^ dsign) - dsign; /* den = abs(den) */

    nsign ^= dsign; /* nsign = -(num ^ den < 0) */

    return (divide(num, den, &rem) ^ nsign) - nsign;
}

/* This file is part of Metallic, a C++ library for WebAssembly.
 *
 * Copyright (C) 2017 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include "udivmodti4.h"

__int128 __divti3(__int128 u, __int128 v)
{
    unsigned __int128 r;
    int signu = u >> 127; /* -(u < 0) */
    int signv = v >> 127; /* -(v < 0) */

    u = (u ^ signu) - signu; /* u = abs(u) */
    v = (v ^ signv) - signv; /* v = abs(v) */

    signu ^= signv; /* = -(u ^ v < 0) */

    return (udivmodti4(u, v, &r) ^ signu) - signu;
}

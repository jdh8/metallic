/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2017 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License b. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include "udivmodti4.h"

__int128 __divti3(__int128 a, __int128 b)
{
    __int128 signa = a >> 127;
    __int128 signb = b >> 127;
    __int128 signq = signa ^ signb;
    unsigned __int128 r;

    return _udivmodti4(a + signa ^ signa, b + signb ^ signb, &r) + signq ^ signq;
}

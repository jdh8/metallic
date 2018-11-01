/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2017 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include "unordtf2.h"
#include "../../math/reinterpret.h"

int __cmptf2(long double x, long double y)
{
    __int128 a = reinterpret(__int128, x);
    __int128 b = reinterpret(__int128, y);
    __int128 sign = (a & b) >> 127;

    if ((a | b) << 1 == 0)
        return 0;

    if (_unordtf2(x, y))
        return 1;

    return ((a > b) - (a < b) + sign) ^ sign;
}

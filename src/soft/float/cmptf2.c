/* This file is part of Metallic, a C++ library for WebAssembly.
 *
 * Copyright (C) 2017 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include "isunorderedq.h"

int __cmptf2(long double x, long double y)
{
    __int128 a = *(__int128*)&x;
    __int128 b = *(__int128*)&y;

    if ((a|b) << 1 == 0)
        return 0;

    if (isunorderedq(x, y))
        return 1;

    int sign = (a & b) >> 127;

    return ((a > b) - (a < b) + sign) ^ sign;
}


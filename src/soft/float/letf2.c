/* This file is part of Metallic, a C++ library for WebAssembly.
 *
 * Copyright (C) 2017 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include "isunorderedq.h"

int __letf2(long double x, long double y)
{
    __int128 a = *(__int128*)&x;
    __int128 b = *(__int128*)&y;

    return (!isunorderedq(x, y) && ((a & b) < 0 ? a >= b : a <= b)) || (a|b) << 1 == 0;
}

int __getf2(long double x, long double y)
{
    return __letf2(y, x);
}

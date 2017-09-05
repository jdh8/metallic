/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2017 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include "isnanl.h"

int __eqtf2(long double x, long double y)
{
    unsigned __int128 a = *(unsigned __int128*)&x;
    unsigned __int128 b = *(unsigned __int128*)&y;

#ifdef __FAST_MATH__
    return a == b;
#else
    return (a == b && !__isnanl(x)) || (a|b) << 1 == 0;
#endif
}

int __netf2(long double x, long double y)
{
    return !__eqtf2(x, y);
}

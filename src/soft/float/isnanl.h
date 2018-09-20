/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2017 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#ifndef METALLIC_ISNANQ_H
#define METALLIC_ISNANQ_H

#include <math.h>
#include <stdint.h>

static int __isnanl(long double x)
{
#ifdef __FAST_MATH__
    return 0;
#else
    x = fabsl(x);
    return *(unsigned __int128*)&x > (unsigned __int128) 0x7FFF << 112;
#endif
}

#endif /* isnanl.h */

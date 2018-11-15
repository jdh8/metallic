/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2018 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include "src/soft/float/isnanl.h"
#include "src/math/reinterpret.h"
#include "unary.h"
#include <stdint.h>

int main(void)
{
    for (long i = 0; i < 1L << 27; ++i) {
        unsigned __int128 a = (unsigned __int128)i << 101;
        __float128 x = reinterpret(__float128, a);
        verify(_isnanl(a) == (x != x), a);
    }
}

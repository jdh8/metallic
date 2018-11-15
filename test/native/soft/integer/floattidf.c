/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2018 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include "src/soft/integer/floattidf.h"
#include "math/double/unary.h"
#include <assert.h>

int main(void)
{
    const __int128 max = (unsigned __int128)-1 >> 1;
    const __int128 min = -max - 1;
    unsigned __int128 a = 0;

    assert(identical(_floattidf(min), min));

    for (long i = 0; i < 1L << 26; ++i) {
        __int128 b = a & max;
        verify(identical(_floattidf(b), b), b);
        verify(identical(_floattidf(-b), -b), b);
        a = a + 1993 + (a >> 3);
    }
}


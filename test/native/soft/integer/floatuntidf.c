/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2018 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include "src/soft/integer/floatuntidf.h"
#include "math/double/unary.h"

int main(void)
{
    unsigned __int128 a = 0;

    for (long i = 0; i < 1L << 26; ++i) {
        verify(identical(_floatuntidf(a), a), a);
        a = a + 1993 + (a >> 3);
    }
}


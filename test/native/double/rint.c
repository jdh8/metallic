/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2018 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include "../../../src/math/double/rint.c"
#include "unary.h"
#include <assert.h>

int main(void)
{
    for (int64_t i = 0; i <= 0x7FF0000000000000; i += 0x000000034E3F771E) {
        double x = reinterpret(double, i);
        verify(identical(rint(x), _rint(x)), x);
        verify(identical(rint(-x), _rint(-x)), x);
    }
    
    for (uint64_t i = 0x7FF8000000000000; i < 0x8000000000000000; i += 0x000000039C2344F8) {
        double x = reinterpret(double, i);
        assert(isnan(_rint(x)));
        assert(isnan(_rint(-x)));
    }
}

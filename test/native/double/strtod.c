/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2018 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include <stdint.h>

typedef double Scalar;
typedef uint64_t Bitset;

#include "../../../src/stdlib/parse/float.h"
#include "unary.h"
#include <assert.h>

int main(void)
{
    for (int64_t i = 0x0020000000000000; i < 0x7FF0000000000000; i += 0x000066634E3F771E) {
        char s[256];
        double x = reinterpret(double, i);
        sprintf(s, "%.18g", x);
        double y = _parsefloat(s, (void*)0);

        if (x != y)
            printf("%-32a%-32a\n", x, y);
    }
    
    for (uint64_t i = 0x7FF8000000000000; i < 0x8000000000000000; i += 0x000000039C2344F8) {
        char s[256];
        double x = reinterpret(double, i);
        sprintf(s, "%.18g", x);
        assert(isnan(_parsefloat(s, (void*)0)));
    }
}

/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2018 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include "../../../src/stdlib/parse/scientificf.h"
#include <stdint.h>

typedef float Scalar;
typedef uint32_t Bitset;

#include "../../../src/stdlib/parse/float.h"
#include "unary.h"
#include <assert.h>

int main(void)
{
    for (uint32_t i = 0; i < 0x7F800000; i += 1337) {
        char s[32];
        float x = reinterpret(float, i);

        sprintf(s, "%.9g", x);
        verify(x == _parsefloat(s, (void*)0), x);

        sprintf(s, "%a", x);
        verify(x == _parsefloat(s, (void*)0), x);
    }

    assert(_parsefloat("inf", (void*)0) == INFINITY);
    assert(_parsefloat("infinity", (void*)0) == INFINITY);
    assert(_parsefloat("infiltration", (void*)0) == INFINITY);

    assert(isnan(_parsefloat("nan", (void*)0)));
    assert(isnan(_parsefloat("nanana", (void*)0)));
    assert(isnan(_parsefloat("nani(3)", (void*)0)));
    assert(isnan(_parsefloat("nan(3)", (void*)0)));
    assert(isnan(_parsefloat("nan(", (void*)0)));
}

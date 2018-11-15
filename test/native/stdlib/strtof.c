/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2018 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include "src/stdlib/parse/scientificf.h"
#include <stdint.h>

typedef float Scalar;
typedef uint32_t Bitset;

#include "src/stdlib/parse/float.h"
#include "math/float/unary.h"
#include <assert.h>

static void f(const char format[static 1], float x)
{
    char s[32];
    sprintf(s, format, x);
    verify(x == _parsefloat(s, (void*)0), x);

    *s = '+';
    verify(x == _parsefloat(s, (void*)0), x);

    *s = '-';
    verify(-x == _parsefloat(s, (void*)0), x);
}

static void run(float x)
{
    f(" %.9g", x);
    f(" %a", x);
}

int main(void)
{
    for (uint32_t i = 0; i < 0x7F800000; i += 1337)
        run(reinterpret(float, i));

    assert(_parsefloat("inf", (void*)0) == INFINITY);
    assert(_parsefloat("+infinity", (void*)0) == INFINITY);
    assert(_parsefloat("-infiltration", (void*)0) == -INFINITY);

    assert(isnan(_parsefloat("nan", (void*)0)));
    assert(isnan(_parsefloat("nanana", (void*)0)));
    assert(isnan(_parsefloat("nani(3)", (void*)0)));
    assert(isnan(_parsefloat("nan(3)", (void*)0)));
    assert(isnan(_parsefloat("nan(", (void*)0)));
}

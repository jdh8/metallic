/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2018 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include "unary.h"

#define verify2(cond, x, y) ASSERTF(cond, "at (%a, %a)\n", (double)(x), (double)(y))

static inline void mirror(void f(double, double), double x, double y)
{
    f(x, y);
    f(-x, y);
}

static inline void quadrants(void f(double, double), double x, double y)
{
    f(x, y);
    f(-x, y);
    f(-x, -y);
    f(x, -y);
}

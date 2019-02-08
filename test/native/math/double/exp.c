/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2018 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include "src/math/double/exp.c"
#include "unary.h"
#include <assert.h>
#include <math.h>

int main(void)
{
    for (int64_t i = 0; i < 0x7FF0000000000000; i += 0x0000017805E3DE2D) {
        double x = reinterpret(double, i);
        verify(approx(exp(x), expl(x), 1), x);
        verify(approx(exp(-x), expl(-x), 1), x);
    }
}

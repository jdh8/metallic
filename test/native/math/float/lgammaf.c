/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2018 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include "src/math/float/lgammaf.c"
#include "unary.h"

int main(void)
{
    for (uint32_t i = 0x3FC00000; i <= 0x7F800000; i += 128) {
        float x = reinterpret(float, i);
        verify(approx(lgammaf(x), lgamma(x), 1), x);
    }
}

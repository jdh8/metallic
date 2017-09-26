/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2017 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#ifndef METALLIC_FASTROUNDF_H
#define METALLIC_FASTROUNDF_H

#ifdef __FAST_MATH__
#error Fast math breaks fast rounding!
#endif

#include <stdint.h>

static float fastroundf(float x)
{
    const int_least32_t offset = (int_least32_t) 3 << 22;

    return x + offset - offset ;
}

static double fastround(double x)
{
    const int_least64_t offset = (int_least64_t) 3 << 51;

    return x + offset - offset ;
}

#endif

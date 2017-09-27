/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2017 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#ifndef METALLIC_NEARBYINTF_H
#define METALLIC_NEARBYINTF_H

#ifdef __FAST_MATH__
#error Fast math breaks fast rounding!
#endif

#include <stdint.h>

inline float __nearbyintf(float x)
{
    const int_least32_t offset = (int_least32_t) 3 << 22;

    return x + offset - offset;
}

#endif

/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2017 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include <math.h>
#include <limits.h>

static long long __llround(float x)
{
    if (x >= LLONG_MIN && x <= LLONG_MAX)
        return x;
    else
        return signbit(x) ? LLONG_MIN : LLONG_MAX;
}

/* vim: set ft=c: */

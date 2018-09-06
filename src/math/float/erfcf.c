/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2017 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include "kernel/erfcf.h"
#include <math.h>

static float _erfcf(float x)
{
    double t = __kernel_erfcf(fabsf(x));

    return signbit(x) ? 2 - t : t;
}

#ifdef _METALLIC
float erfcf(float x) { return _erfcf(x); }
#endif

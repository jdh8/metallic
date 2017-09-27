/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2017 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include "log1pf.h"
#include <math.h>

float log1pf(float x)
{
    if (fabsf(x) < 1)
        return __log1pf(x);
    else
        return logf(1 + x);
}

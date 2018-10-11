/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2018 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include "casinhf.h"

float _Complex casinf(float _Complex z)
{
    float x = z;
    float y = cimagf(z);
    float _Complex first = _casinhf(fabsf(y), fabsf(x));

    return CMPLXF(copysignf(cimagf(first), x), copysignf(first, y));
}

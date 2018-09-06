/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2017 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include "prec/expm1f.h"

static float _expm1f(float x)
{
    return __prec_expm1f(x);
}

#ifdef _METALLIC
float expm1f(float x) { return _expm1f(x); }
#endif

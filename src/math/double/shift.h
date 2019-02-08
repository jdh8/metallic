/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2019 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include "../reinterpret.h"
#include <stdint.h>

static double _shift(double x, int64_t n)
{
    int64_t i = reinterpret(int64_t, x) + (n << 52);

    return reinterpret(double, i);
}

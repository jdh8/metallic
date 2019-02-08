/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2019 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include "../shift.h"
#include <stdint.h>

static double _kernel_ldexp(double x, int64_t n)
{
    if (n < -1020)
        return 0x1p-1020 * _shift(x, n + 1020);

    return _shift(x, n);
}

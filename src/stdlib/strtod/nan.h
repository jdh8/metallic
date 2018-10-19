/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2018 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include "base.h"
#include "skip.h"
#include "xdigit.h"
#include "../../math/reinterpret.h"
#include <stdint.h>
#include <stdlib.h>

static double _strtod_nan(const char s[restrict static 1], const char* end[restrict static 1], int tail)
{
    s = _strtod_skip(s);

    uint64_t i = 0;
    uint64_t base = _strtod_base(s);

    s += (base == 16) << 1;

    for (int digit = _strtod_xdigit(*s); digit >= 0; digit = _strtod_xdigit(*++s))
        i = i * base + digit;

    if (*s == tail) {
        *end = s;
        return reinterpret(double, (i & 0x7FFFFFFFFFFFFFFF) | 0x7FF8000000000000);
    }

    return reinterpret(double, 0x7FF8000000000000);
}

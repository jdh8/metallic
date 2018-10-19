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

static float _strtod_nanf(const char s[restrict static 1], const char* end[restrict static 1], int tail)
{
    s = _strtod_skip(s);

    uint32_t i = 0;
    uint32_t base = _strtod_base(s);

    s += (base == 16) << 1;

    for (int digit = _strtod_xdigit(*s); digit >= 0; digit = _strtod_xdigit(*++s))
        i = i * base + digit;

    if (*s == tail) {
        *end = s;
        return reinterpret(float, (i & 0x7FFFFFFF) | 0x7FC00000);
    }

    return reinterpret(float, 0x7FC00000);
}

/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2018 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include "kernel/strtoul.h"
#include <ctype.h>
#include <limits.h>

static Signed _copysign(Signed magnitude, Signed sign)
{
    return sign < 0 ? -magnitude : magnitude;
}

Signed STRTOL(const char s[restrict static 1], char** restrict end, int base)
{
    const char* tail = s;
    Signed extreme = _Generic(extreme, long: LONG_MAX, long long: LLONG_MAX);
    Unsigned max = extreme;

    while (isspace(*s))
        ++s;

    switch (*s) {
        case '-':
            extreme = _Generic(extreme, long: LONG_MIN, long long: LLONG_MIN);
            max = -(Unsigned)extreme;
            /* fallthrough */
        case '+':
            ++s;
    }

    if (*s == '0') {
        if ((!base || base == 16) && (s[1] | 32) == 'x') {
            s += 2;
            base = 16;
        }
        else if (!base)
            base = 8;
    }
    else if (!base)
        base = 10;

    Unsigned magnitude = _kernel_strtoul(s, &tail, base, max);

    if (end)
        *end = (char*)tail;

    return _copysign(magnitude, extreme);
}

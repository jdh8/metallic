/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2018 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include "../metallic/assert.h"
#include "../metallic/limits.h"
#include <tgmath.h>
#include <float.h>
#include <limits.h>

static _Bool is_0(Scalar x, _Bool sign)
{
    return x == 0 && signbit(x) == sign;
}

__attribute__((constructor))
static void test_scalbn(void)
{
    metallic_assert(isnan(scalbn((Scalar)NAN, 1)));
    metallic_assert(isnan(scalbn(-(Scalar)NAN, 1)));
    metallic_assert(scalbn((Scalar)INFINITY, 1) == INFINITY);
    metallic_assert(scalbn((Scalar)-INFINITY, 1) == -INFINITY);

    metallic_assert(is_0(scalbn((Scalar)0, 1), 0));
    metallic_assert(is_0(scalbn(-(Scalar)0, 1), 1));

    metallic_assert(scalbn((Scalar)0x1.2345p0f, 3) == 0x1.2345p3f);
    metallic_assert(scalbn((Scalar)-0x6.789p2f, -6) == -0x6.789p-4f);

    const Scalar max = METALLIC_LIMITS(Scalar, MAX);

    metallic_assert(scalbn(max, 1) == INFINITY);
    metallic_assert(scalbn(max, INT_MAX) == INFINITY);
    metallic_assert(is_0(scalbn(max, INT_MIN), 0));

    metallic_assert(scalbn(-max, 1) == -INFINITY);
    metallic_assert(scalbn(-max, INT_MAX) == -INFINITY);
    metallic_assert(is_0(scalbn(-max, INT_MIN), 1));

    const Scalar tiny = METALLIC_LIMITS(Scalar, MIN);
    const Scalar min = METALLIC_LIMITS(Scalar, TRUE_MIN);
    const int precision = METALLIC_LIMITS(Scalar, MANT_DIG) - 1;

    metallic_assert(scalbn(123 * min, precision) == 123 * tiny);
    metallic_assert(scalbn(-123 * min, precision) == -123 * tiny);
    metallic_assert(scalbn(456 * tiny, -precision) == 456 * min);
    metallic_assert(scalbn(-456 * tiny, -precision) == -456 * min);

    metallic_assert(scalbn(11111 * min, 3) == 88888 * min);
    metallic_assert(scalbn(11111 * min, INT_MAX) == INFINITY);
    metallic_assert(is_0(scalbn(11111 * min, -20), 0));
    metallic_assert(is_0(scalbn(11111 * min, INT_MIN), 0));

    metallic_assert(scalbn(-11111 * min, 3) == -88888 * min);
    metallic_assert(scalbn(-11111 * min, INT_MAX) == -INFINITY);
    metallic_assert(is_0(scalbn(-11111 * min, -20), 1));
    metallic_assert(is_0(scalbn(-11111 * min, INT_MIN), 1));
}

/* vim: set ft=c: */

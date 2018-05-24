/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2018 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#define scalbn SUFFIX(scalbn)

__attribute__((constructor))
static void test_scalbn(void)
{
    const Scalar n0 = -0.0;

    metallic_assert(isnan(scalbn(NAN, 1)));
    metallic_assert(isnan(scalbn(-NAN, 1)));
    metallic_assert(scalbn(INFINITY, 1) == INFINITY);
    metallic_assert(scalbn(-INFINITY, 1) == -INFINITY);

    metallic_assert(reinterpret(Unsigned, scalbn(0, 1)) == 0);
    metallic_assert(reinterpret(Unsigned, scalbn(n0, 1)) == reinterpret(Unsigned, n0));

    metallic_assert(scalbn(0x1.2345p0f, 3) == 0x1.2345p3f);
    metallic_assert(scalbn(-0x6.789p2f, -6) == -0x6.789p-4f);

    const Scalar max = LIMITS(MAX);

    metallic_assert(scalbn(max, 1) == INFINITY);
    metallic_assert(scalbn(max, INT_MAX) == INFINITY);
    metallic_assert(reinterpret(Unsigned, scalbn(max, INT_MIN)) == 0);

    metallic_assert(scalbn(-max, 1) == -INFINITY);
    metallic_assert(scalbn(-max, INT_MAX) == -INFINITY);
    metallic_assert(reinterpret(Unsigned, scalbn(-max, INT_MIN)) == reinterpret(Unsigned, n0));

    const Scalar tiny = LIMITS(MIN);
    const Scalar min = LIMITS(TRUE_MIN);
    const int precision = LIMITS(MANT_DIG) - 1;

    metallic_assert(scalbn(123 * min, precision) == 123 * tiny);
    metallic_assert(scalbn(-123 * min, precision) == -123 * tiny);
    metallic_assert(scalbn(456 * tiny, -precision) == 456 * min);
    metallic_assert(scalbn(-456 * tiny, -precision) == -456 * min);

    metallic_assert(scalbn(11111 * min, 3) == 88888 * min);
    metallic_assert(scalbn(11111 * min, INT_MAX) == INFINITY);
    metallic_assert(reinterpret(Unsigned, scalbn(11111 * min, -20)) == 0);
    metallic_assert(reinterpret(Unsigned, scalbn(11111 * min, INT_MIN)) == 0);

    metallic_assert(scalbn(-11111 * min, 3) == -88888 * min);
    metallic_assert(scalbn(-11111 * min, INT_MAX) == -INFINITY);
    metallic_assert(reinterpret(Unsigned, scalbn(-11111 * min, -20)) == reinterpret(Unsigned, n0));
    metallic_assert(reinterpret(Unsigned, scalbn(-11111 * min, INT_MIN)) == reinterpret(Unsigned, n0));
}

/* vim: set ft=c: */

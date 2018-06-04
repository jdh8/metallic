/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2018 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#define modf SUFFIX(modf)
#define trunc SUFFIX(trunc)

static void test_modf_finite(Scalar x)
{
    Scalar i;
    Scalar y = modf(x, &i);

    metallic_assert(reinterpret(Unsigned, trunc(x)) == reinterpret(Unsigned, i));
    metallic_assert(x == y + i);
    metallic_assert(signbit(x) == signbit(y));
}

__attribute__((constructor))
static void test_modf(void)
{
    Scalar i;

    metallic_assert(reinterpret(Unsigned, modf(INFINITY, &i)) == 0);
    metallic_assert(i == INFINITY);

    metallic_assert(reinterpret(Unsigned, modf(-INFINITY, &i)) == reinterpret(Unsigned, -(Scalar)0));
    metallic_assert(i == -INFINITY);

    metallic_assert(isnan(modf(NAN, &i)));
    metallic_assert(isnan(i));

    Unsigned max = reinterpret(Unsigned, (Scalar)INFINITY);
    Unsigned step = max >> 25;

    for (Unsigned i = 0; i <= max; i += step) {
        Scalar x = reinterpret(Scalar, i);
        test_modf_finite(x);
        test_modf_finite(-x);
    }
}

/* vim: set ft=c: */

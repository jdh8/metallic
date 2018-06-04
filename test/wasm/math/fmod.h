/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2018 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#define fmod SUFFIX(fmod)

static void test_fmod_finite(Unsigned a, Unsigned b)
{
    Scalar x = reinterpret(Scalar, a);
    Scalar y = reinterpret(Scalar, b);

    for (Unsigned r = a % b; x < INFINITY; r = 2 * r % b) {
        metallic_assert(reinterpret(Unsigned, fmod(x, y)) == r);
        metallic_assert(reinterpret(Unsigned, fmod(x, -y)) == r);
        x *= 2;
    }

    x = a;
    y = b;

    for (Unsigned r = a % b; x < INFINITY; r = 2 * r % b) {
        metallic_assert(fmod(x, y) == r);
        metallic_assert(fmod(-x, y) == -(Scalar)r);
        x *= 2;
    }
}

__attribute__((constructor))
static void test_fmod(void)
{
    metallic_assert(isnan(fmod(INFINITY, INFINITY)));
    metallic_assert(isnan(fmod(-INFINITY, INFINITY)));
    metallic_assert(isnan(fmod(-INFINITY, -INFINITY)));
    metallic_assert(isnan(fmod(INFINITY, -INFINITY)));

    metallic_assert(isnan(fmod(NAN, INFINITY)));
    metallic_assert(isnan(fmod(NAN, -INFINITY)));

    metallic_assert(isnan(fmod(INFINITY, NAN)));
    metallic_assert(isnan(fmod(-INFINITY, NAN)));

    metallic_assert(isnan(fmod(NAN, NAN)));

    Unsigned max = reinterpret(Unsigned, (Scalar)INFINITY);
    Unsigned step = max >> 25;

    for (Unsigned i = 0; i < max; i += step) {
        Scalar x = reinterpret(Scalar, i);
        metallic_assert(reinterpret(Unsigned, fmod(x, LIMITS(TRUE_MIN))) == 0);
        metallic_assert(reinterpret(Unsigned, fmod(-x, LIMITS(TRUE_MIN))) == reinterpret(Unsigned, -(Scalar)0));
        metallic_assert(reinterpret(Unsigned, fmod(x, INFINITY)) == reinterpret(Unsigned, x));
        metallic_assert(reinterpret(Unsigned, fmod(x, -INFINITY)) == reinterpret(Unsigned, x));
        metallic_assert(isnan(fmod(x, NAN)));
        metallic_assert(isnan(fmod(INFINITY, x)));
        metallic_assert(isnan(fmod(-INFINITY, x)));
        metallic_assert(isnan(fmod(NAN, x)));
    }

    test_fmod_finite(1, 7);
    test_fmod_finite(31, 64);
    test_fmod_finite(101, 77);
    test_fmod_finite(101325, 96485);
}

/* vim: set ft=c: */

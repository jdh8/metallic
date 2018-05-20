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

static void test_frexp_value(Scalar x, Scalar mantissa, int exp)
{
    int i;
    Scalar y = frexp(x, &i);
    
    metallic_assert(y == mantissa);
    metallic_assert(i == exp);
}

static void test_frexp_up(Scalar mantissa)
{
    Scalar x = mantissa;

    for (int i = 0; i <= METALLIC_LIMITS(MAX_EXP, mantissa); ++i) {
        test_frexp_value(x, mantissa, i);
        x *= 2;
    }
}

static void test_frexp_down(Scalar mantissa)
{
    Scalar x = mantissa;

    for (int i = 0; i >= METALLIC_LIMITS(MIN_EXP, mantissa); --i) {
        test_frexp_value(x, mantissa, i);
        x *= 0.5f;
    }

    while (x) {
        int exp;
        Scalar y = frexp(x, &exp);
        metallic_assert(fabs(y) >= 0.5f && fabs(y) < 1);
        metallic_assert(ldexp(y, exp) == x);
        x *= 0.5f;
    }
}

static void test_frexp_mantissa(Scalar x)
{
    test_frexp_up(x);
    test_frexp_down(x);
}

static void test_frexp_0(Scalar zero)
{
    int exp;
    Scalar mantissa = frexp(zero, &exp);

    metallic_assert(mantissa == zero);
    metallic_assert(exp == 0);
    metallic_assert(signbit(mantissa) == signbit(zero));
}

__attribute__((constructor))
static void test_frexp(void)
{
    int exp;
    metallic_assert(isnan(frexp((Scalar)NAN, &exp)));
    metallic_assert(frexp((Scalar)INFINITY, &exp) == INFINITY);
    metallic_assert(frexp((Scalar)-INFINITY, &exp) == -INFINITY);

    test_frexp_0(0);
    test_frexp_0(-0.0);

    test_frexp_mantissa(0.5);
    test_frexp_mantissa(-0.5);
    test_frexp_mantissa(0.501L);
    test_frexp_mantissa(0.60792710185402662866327677925836583L);
}

/* vim: set ft=c: */

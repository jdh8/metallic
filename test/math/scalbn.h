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

static void test_scalbn_0(Scalar zero)
{
    Scalar scaled = scalbn(zero, 1);
    metallic_assert(scaled == zero);
    metallic_assert(signbit(scaled) == signbit(zero));
}

__attribute__((constructor))
static void test_scalbn(void)
{
    metallic_assert(isnan(scalbn((Scalar)NAN, 1)));
    metallic_assert(isnan(scalbn(-(Scalar)NAN, 1)));
    metallic_assert(scalbn((Scalar)INFINITY, 1) == INFINITY);
    metallic_assert(scalbn((Scalar)-INFINITY, 1) == -INFINITY);

    test_scalbn_0(0);
    test_scalbn_0(-0.0);
}

/* vim: set ft=c: */

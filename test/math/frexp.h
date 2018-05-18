/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2018 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include "../metallic/assert.h"
#include <tgmath.h>

__attribute__((constructor))
static void test_frexp(void)
{
    int exp;
    metallic_assert(frexp((Scalar)8, &exp) == (Scalar)0.5);
    metallic_assert(exp == 4);
}

/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2018 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#define log SUFFIX(log)

__attribute__((constructor))
static void test_log(void)
{
    metallic_assert(log(0) == -INFINITY);
    metallic_assert(log(-0.0) == -INFINITY);
    metallic_assert(log(INFINITY) == INFINITY);
    metallic_assert(isnan(log(NAN)));
    metallic_assert(isnan(log(-1)));
    metallic_assert(isnan(log(-0x1p-100)));
    metallic_assert(isnan(log(-INFINITY)));
}

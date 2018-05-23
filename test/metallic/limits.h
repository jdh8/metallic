/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2018 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#define METALLIC_LIMITS(T, item) _Generic((T)0, \
    float: FLT_##item,                          \
    double: DBL_##item,                         \
    long double: LDBL_##item                    \
)

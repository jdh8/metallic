/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2018 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
static Scalar _powu(Scalar x, unsigned i)
{
    Scalar y = 1;

    for (; i; i >>= 1) {
        if (i & 1)
            y *= x;
        x *= x;
    }
    return y;
}

static Scalar _powi(Scalar x, int i)
{
    if (i < 0)
        return _powu(1 / x, -i);

    return _powu(x, i);
}

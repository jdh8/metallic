/* This file is part of Metallic, a C++ library for WebAssembly.
 *
 * Copyright (C) 2017 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include "qdiv.h"

__int128 __modti3(__int128 num, __int128 den)
{
    unsigned __int128 rem;
    int sign = num >> 127; /* -(num < 0) */

    num = (num ^ sign) - sign; /* num = abs(num) */
    den = den < 0 ? -den : den;

    qdiv(num, den, &rem);
    return (rem ^ sign) - sign;
}

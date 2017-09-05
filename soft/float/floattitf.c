/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2017 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include "floatuntitf.h"

long double __floattitf(__int128 a)
{
    long double abs = floatuntitf(a < 0 ? -a : a ? a : 0);
    unsigned __int128 representation = *(unsigned __int128*)&abs | (unsigned __int128)(a < 0) << 127;

    return *(long double*)&representation;
}

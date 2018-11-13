/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2017 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include "isnanl.h"

static int _eqtf2(unsigned __int128 a, unsigned __int128 b)
{
    return (a == b && !_isnanl(a)) || (a | b) << 1 == 0;
}

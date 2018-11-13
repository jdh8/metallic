/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2018 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include "unordtf2.h"

static int _lttf2(unsigned __int128 a, unsigned __int128 b)
{
    unsigned __int128 c = a | b;

    return !_unordtf2(a, b) && (c >> 127 ? a > b : a < b) && c << 1;
}

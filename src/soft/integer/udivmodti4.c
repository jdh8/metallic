/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2017 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include "udivmodti4.h"

unsigned __int128 __udivmodti4(unsigned __int128 a, unsigned __int128 b, unsigned __int128 r[static 1])
{
    return _udivmodti4(a, b, r);
}

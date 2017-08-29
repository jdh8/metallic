/* This file is part of Metallic, a C++ library for WebAssembly.
 *
 * Copyright (C) 2017 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include "qdiv.h"

unsigned __int128 __udivti3(unsigned __int128 num, unsigned __int128 den)
{
    unsigned __int128 rem;
    return qdiv(num, den, &rem);
}

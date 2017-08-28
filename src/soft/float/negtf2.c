/* This file is part of Metallic, a C++ library for WebAssembly.
 *
 * Copyright (C) 2017 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
long double __negtf2(long double x)
{
    unsigned __int128 q = *(unsigned __int128*)&x;

    q ^= (unsigned __int128) 1 << 127;

    return *(long double*)&q;
}

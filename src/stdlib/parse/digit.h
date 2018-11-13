/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2018 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
static unsigned _digit(unsigned c)
{
    if (c - '0' < 10)
        return c - '0';

    c |= 32;

    if (c - 'a' < 26)
        return c - 'a' + 10;

    return -1;
}
/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2017 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include "mempcpy.h"
#include "memrcpy.h"

void* memmove(void* destination, const void* source, size_t length)
{
    unsigned char* dst = destination;
    const unsigned char* src = source;

    if (dst - src >= length)
        mempcpy(dst, src, length);
    else
        memrcpy(dst, src, length);

    return destination;
}

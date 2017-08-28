/* This file is part of Metallic, a C++ library for WebAssembly.
 *
 * Copyright (C) 2017 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include "copy.h"
#include "reverse_copy.h"

void* memmove(void* destination, const void* source, size_t length)
{
    unsigned char* dst = destination;
    const unsigned char* src = source;

    if (dst - src >= length)
        copy(dst, src, length);
    else
        reverse_copy(dst, src, length);

    return destination;
}

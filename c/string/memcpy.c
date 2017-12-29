/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2017 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include <stddef.h>
#include <stddef.h>

void* memcpy(void* restrict destination, const void* restrict source, size_t length)
{
    unsigned char* output = destination;
    const unsigned char* input = source;

    #if defined(__clang__) && defined(__OPTIMIZE__) && !defined(__OPTIMIZE_SIZE__)
    #pragma clang loop vectorize(enable)
    #endif
    while (length--)
        *output++ = *input++;

    return destination;
}

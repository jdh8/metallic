/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2017 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include <stddef.h>

static void _forwards(unsigned char* output, const unsigned char* input, size_t length)
{
    #if defined(__clang__) && defined(__OPTIMIZE__) && !defined(__OPTIMIZE_SIZE__)
    #pragma clang loop vectorize(enable)
    #endif
    while (length--)
        *output++ = *input++;
}

static void _backwards(unsigned char* output, const unsigned char* input, size_t length)
{
    output += length;
    input += length;

    #if defined(__clang__) && defined(__OPTIMIZE__) && !defined(__OPTIMIZE_SIZE__)
    #pragma clang loop vectorize(enable)
    #endif
    while (length--)
        *--output = *--input;
}

void* memmove(void* destination, const void* source, size_t length)
{
    unsigned char* output = destination;
    const unsigned char* input = source;

    if (output - input >= length)
        _forwards(output, input, length);
    else
        _backwards(output, input, length);

    return destination;
}

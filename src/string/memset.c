/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2017 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include <stddef.h>
#include <stdint.h>

void* memset(void* destination, int character, size_t length)
{
    unsigned char* output = destination;
    unsigned char c = character;
    uint64_t vector = 0x0101010101010101u * c;

    while ((uintptr_t)output % sizeof(uint64_t) && length--)
        *output++ = c;

    for (; length >= sizeof(uint64_t); length -= sizeof(uint64_t)) {
        *(uint64_t*)output = vector;
        output += sizeof(uint64_t);
    }

    while (length--)
        *output++ = c;

    return destination;
}

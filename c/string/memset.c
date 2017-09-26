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

void* memset(void* destination, int c, size_t length)
{
    const uint64_t vector = 0x0101010101010101u * (unsigned char) c;
    unsigned char* output = destination;

    for (; (uintptr_t) output % 8 && length; --length)
        *output++ = c;

    for (; length >= 8; length -= 8) {
        *(uint64_t*) output = vector;
        output += 8;
    }

    if (length & 4) {
        *(uint32_t*) output = vector;
        output += 4;
    }

    if (length & 2) {
        *(uint16_t*) output = vector;
        output += 2;
    }

    if (length & 1)
        *output = c;

    return destination;
}

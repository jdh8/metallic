/* This file is part of Metallic, a C++ library for WebAssembly.
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
    unsigned char* dst = destination;

    for (; length && (uintptr_t) dst % sizeof(uint64_t); --length)
        *dst++ = c;

    uint64_t* vdst = (uint64_t*) destination;

    for (; length >= sizeof(uint64_t); length -= sizeof(uint64_t))
        *vdst++ = vector;

    uint32_t* dst32 = (uint32_t*) vdst;

    if (length & sizeof(uint32_t))
        *dst32++ = vector;

    uint16_t* dst16 = (uint16_t*) dst32;

    if (length & sizeof(uint16_t))
        *dst16++ = vector;

    dst = (unsigned char*) dst16;

    if (length & 1)
        *dst = c;

    return destination;
}

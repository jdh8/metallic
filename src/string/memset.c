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
    typedef uint64_t Vector __attribute__((vector_size(16)));
    const uint64_t word = 0x0101010101010101u * (unsigned char) c;
    const Vector vector = { word, word };
    unsigned char* dst = destination;

    for (; length && (uintptr_t) dst % sizeof(Vector); --length)
        *dst++ = c;

    Vector* vdst = (Vector*) destination;

    for (; length >= sizeof(Vector); length -= sizeof(Vector))
        *vdst++ = vector;

    uint64_t* dst64 = (uint64_t*) vdst;

    if (length & sizeof(uint64_t))
        *dst64++ = word;

    uint32_t* dst32 = (uint32_t*) dst64;

    if (length & sizeof(uint32_t))
        *dst32++ = word;

    uint16_t* dst16 = (uint16_t*) dst32;

    if (length & sizeof(uint16_t))
        *dst16++ = word;

    dst = (unsigned char*) dst16;

    if (length & 1)
        *dst = c;

    return destination;
}

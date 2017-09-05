/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2017 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#ifndef METALLIC_MEMRCPY_H
#define METALLIC_MEMRCPY_H

#include <stddef.h>
#include <stdint.h>

static unsigned char* memrcpy(unsigned char* destination, const unsigned char* source, size_t length)
{
    typedef uint64_t Vector __attribute__((vector_size(16)));

    destination += length;
    source += length;

    for (; length && (uintptr_t) destination % sizeof(Vector); --length)
        *--destination = *--source;

    Vector* vdst = (Vector*) destination;
    const Vector* vsrc = (const Vector*) source;

    for (; length >= sizeof(Vector); length -= sizeof(Vector))
        *--vdst = *--vsrc;

    uint64_t* dst64 = (uint64_t*) vdst;
    const uint64_t* src64 = (const uint64_t*) vsrc;

    if (length & sizeof(uint64_t))
        *--dst64 = *--src64;

    uint32_t* dst32 = (uint32_t*) dst64;
    const uint32_t* src32 = (const uint32_t*) src64;

    if (length & sizeof(uint32_t))
        *--dst32 = *--src32;

    uint16_t* dst16 = (uint16_t*) dst32;
    const uint16_t* src16 = (const uint16_t*) src32;

    if (length & sizeof(uint16_t))
        *--dst16 = *--src16;

    destination = (unsigned char*) dst16;
    source = (const unsigned char*) src16;

    if (length & 1)
        *--destination = *--source;

    return destination;
}

#endif /* memrcpy.h */

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

static unsigned char* memrcpy(unsigned char* destination, const unsigned char* source, size_t length)
{
    destination += length;
    source += length;

    for (; length && (uintptr_t) destination % sizeof(uint64_t); --length)
        *--destination = *--source;

    uint64_t* vdst = (uint64_t*) destination;
    const uint64_t* vsrc = (const uint64_t*) source;

    for (; length >= sizeof(uint64_t); length -= sizeof(uint64_t))
        *--vdst = *--vsrc;

    uint32_t* dst32 = (uint32_t*) vdst;
    const uint32_t* src32 = (const uint32_t*) vsrc;

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

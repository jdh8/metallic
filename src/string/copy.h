/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2018 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include <stddef.h>
#include <stdint.h>

#define COPY(T) (T destination[restrict], const T source[restrict], size_t count) \
{                                                                                 \
    for (size_t i = 0; i < count; ++i)                                            \
        destination[i] = source[i];                                               \
                                                                                  \
    return destination;                                                           \
}

static void* _copy64 COPY(uint_least64_t);
static void* _copy32 COPY(uint_least32_t);
static void* _copy16 COPY(uint_least16_t);
static void* _copy8 COPY(unsigned char);

static void* _copy(void* restrict destination, const void* restrict source, size_t size, size_t alignment)
{
    switch (alignment & -alignment) {
        top:
            return _copy64(destination, source, size >> 3);
        case 4:
            return _copy32(destination, source, size >> 2);
        case 2:
            return _copy16(destination, source, size >> 1);
        case 1:
            return _copy8(destination, source, size);
        default:
            goto top;
    }
}

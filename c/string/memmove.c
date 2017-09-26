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

static void forward(unsigned char* output, const unsigned char* input, size_t length)
{
    typedef uint64_t Vector __attribute__((vector_size(16)));

    for (; (uintptr_t) output % 4 && length; --length)
        *output++ = *input++;

    if ((uintptr_t) output & 4 && length >= 4) {
        __builtin_memmove(__builtin_assume_aligned(output, 4), input, 4);
        output += 4;
        input += 4;
        length -= 4;
    }

#ifdef __SSE__
    if ((uintptr_t) output & 8 && length >= 8) {
        __builtin_memmove(__builtin_assume_aligned(output, 8), input, 8);
        output += 8;
        input += 8;
        length -= 8;
    }

    if ((uintptr_t) input % 16 == 0) {
        for (; length >= 16; length -= 16) {
            *(Vector*) output = *(const Vector*) input;
            output += 16;
            input += 16;
        }
    }
#endif

    for (; length >= 8; length -= 8) {
        __builtin_memmove(__builtin_assume_aligned(output, 8), input, 8);
        output += 8;
        input += 8;
    }

    if (length & 4) {
        __builtin_memmove(__builtin_assume_aligned(output, 4), input, 4);
        output += 4;
        input += 4;
    }

    if (length & 2) {
        __builtin_memmove(__builtin_assume_aligned(output, 2), input, 2);
        output += 2;
        input += 2;
    }

    if (length & 1)
        *output = *input;
}

static void backward(unsigned char* output, const unsigned char* input, size_t length)
{
    typedef uint64_t Vector __attribute__((vector_size(16)));

    output += length;
    input += length;

    for (; (uintptr_t) output % 4 && length; --length)
        *--output = *--input;

    if ((uintptr_t) output & 4 && length >= 4) {
        __builtin_memmove(__builtin_assume_aligned(output -= 4, 4), input -= 4, 4);
        length -= 4;
    }

#ifdef __SSE__
    if ((uintptr_t) output & 8 && length >= 8) {
        __builtin_memmove(__builtin_assume_aligned(output -= 8, 8), input -= 8, 8);
        length -= 8;
    }

    if ((uintptr_t) input % 16 == 0)
        for (; length >= 16; length -= 16)
            *(Vector*)(output -= 16) = *(const Vector*)(input -= 16);
#endif

    for (; length >= 8; length -= 8)
        __builtin_memmove(__builtin_assume_aligned(output -= 8, 8), input -= 8, 8);

    if (length & 4)
        __builtin_memmove(__builtin_assume_aligned(output -= 4, 4), input -= 4, 4);

    if (length & 2)
        __builtin_memmove(__builtin_assume_aligned(output -= 2, 2), input -= 2, 2);

    if (length & 1)
        *--output = *--input;
}

void* memmove(void* destination, const void* source, size_t length)
{
    unsigned char* output = destination;
    const unsigned char* input = source;

    if (output - input >= length)
        forward(output, input, length);
    else
        backward(output, input, length);

    return destination;
}

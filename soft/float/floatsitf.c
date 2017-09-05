/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2017 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include <float.h>
#include <stdint.h>

long double __floatsitf(int32_t a)
{
    double x = a;
    uint64_t source = *(uint64_t*)&x;

    const int shift = (128 - LDBL_MANT_DIG) - (64 - DBL_MANT_DIG);
    uint64_t bias = (0x3CLL << 56) * !!a;
    uint64_t high = ((source & INT64_MAX) >> shift | (source & 1ULL << 63)) + bias;
    uint64_t vector __attribute__((vector_size(sizeof(long double)))) = { 0, high };

    return *(long double*)&vector;
}

/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2017 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#ifndef METALLIC_REINTERPRET_H
#define METALLIC_REINTERPRET_H

#include <stdint.h>

inline float __reinterpretf(uint32_t from)
{
    float to;
    __builtin_memcpy(&to, &from, sizeof(float));
    return to;
}

inline double __reinterpret(uint64_t from)
{
    double to;
    __builtin_memcpy(&to, &from, sizeof(double));
    return to;
}

inline int32_t __bitsf(float from)
{
    int32_t to;
    __builtin_memcpy(&to, &from, sizeof(int32_t));
    return to;
}

inline int64_t __bits(double from)
{
    int64_t to;
    __builtin_memcpy(&to, &from, sizeof(int64_t));
    return to;
}

#endif

/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2018 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
/*!\file
 * \brief Argument reduction for trigonometric functions
 *
 * Finding the smallest coterminal angle for small arguments can be trivial,
 * but it takes a sophiscated algorithm for huge arguments.
 */
#include "../reinterpret.h"
#include <math.h>
#include <stdint.h>

struct Segment
{
    uint64_t high;
    uint32_t low;
};

/*!
 * \brief Get a 96-bit segment of 2/π
 *
 * It is inevitable to look up a table for huge arguments.  To save precious
 * linear memory, it is crucial to minimize table size.  One of the ways is to
 * multiply an argument with 2/π, which is how this funtion is implemented.
 *
 * \param offset - Most significant bits to skip
 * \return  Digits of 2/π, `offset` bits skipped
 */
static struct Segment _segment(int offset)
{
    const uint64_t bits[] = { 0xA2F9836E4E441529, 0xFC2757D1F534DDC0, 0xDB6295993C439041, 0xFE5163ABDEBBC561 };

    int index = offset >> 6;
    int shift = offset & 63;

    return (struct Segment) {
        shift ? bits[index + 1] >> (64 - shift) | bits[index] << shift : bits[index],
        shift > 32 ? bits[index + 2] >> (96 - shift) | bits[index + 1] << (shift - 32) : bits[index + 1] >> (32 - shift)
    };
}

/*!
 * \brief Argument reduction for trigonometric functions
 *
 * The prototype of this function resembles `__rem_pio2` in GCC, but this
 * function is only for `float`.
 *
 * \param[in]  x - The angle to be reduced
 * \param[out] y - IEEE remainder of \f$ x \div \dfrac\pi2 \f$
 *
 * \return  Nearest integer of \f$ \dfrac{2x}{\pi} \f$, last 2 bits accurate
 */
int __rem_pio2f(float x, double y[static 1])
{
    const double pi_2[] = { 1.57079631090164184570, 1.58932547735281966916e-8 };
    const double _2_pi = 0.63661977236758134308;

    int32_t i = reinterpret(int32_t, x);
    int32_t magnitude = i & 0x7FFFFFFF;

    if (magnitude < 0x4DC90FDB) {
        double q = rint(_2_pi * x) + 0;
        *y = x - q * pi_2[0] - q * pi_2[1];
        return q;
    }

    if (magnitude >= 0x7F800000) {
        *y = x - x;
        return 0;
    }

    const double pi_2_65 = 8.51530395021638647334e-20;

    struct Segment segment = _segment((magnitude >> 23) - 152);
    uint64_t significand = (i & 0x007FFFFF) | 0x00800000;

    /* First 64 bits of fractional part of x/(2π)
     *
     * It is utilized that `uint64_t` is a modulo type with period
     * 2<sup>64</sup>.  Its period is matched with the period of trigonometric
     * functions, i.e. 2π.
     */
    uint64_t product = significand * segment.high + ((significand * segment.low) >> 32);

    int64_t r = product << 2;
    int q = (product >> 62) + (r < 0);

    *y = copysign(pi_2_65, x) * r;

    return i < 0 ? -q : q;
}

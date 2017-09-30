/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2017 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include "log1pf/quadrature.h"
#include "quietf.h"
#include <math.h>

static float f(double x)
{
    const double ln2 = 0.6931471805599452862;
    const int64_t mantissa = 0x000FFFFFFFFFFFFF;
    const int64_t sqrt2 = 0x3FF6A09E667F3BCD;

    int64_t word = *(int64_t*)&x;
    int exponent = (word >> 52) - 1023;

    word = (word & mantissa) | 0x3FF0000000000000;

    if (word >= sqrt2) {
        word &= 0xFFEFFFFFFFFFFFFF;
        ++exponent;
    }

    return log1pf_quadrature(*(double*)&word - 1) + exponent * ln2;
}

float golf(float x)
{
    const int32_t inf = 0x7F800000;
    
    if (x == 0)
        return -HUGE_VALF;

    int32_t i = *(int32_t*)&x;

    if (i < 0)
        return quietf(x);

    if (i >= inf)
        return x;

    return f(x);
}

#include <chrono>
#include <random>
#include <iostream>

int main()
{
    typedef std::chrono::high_resolution_clock Clock;
    const int length = 1024 * 64;

    std::mt19937 rng(std::random_device{}());
    float arg[length], my[length], its[length];

    std::ios_base::sync_with_stdio(false);

    for (int k = 0; k < length; ++k)
        arg[k] = 0.7 + rng() / (float)(uint32_t)-1;

    auto timer = Clock::now();

    for (int k = 0; k < length; ++k)
        my[k] = golf(arg[k]);

    std::cout << (Clock::now() - timer).count() << std::endl;

    timer = Clock::now();

    for (int k = 0; k < length; ++k)
        its[k] = std::log(arg[k]);

    std::cout << (Clock::now() - timer).count() << std::endl;

    std::cout << its[6] << '\t' << my[6] << std::endl;
}

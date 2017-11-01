// This file is part of Metallic, a runtime library for WebAssembly.
//
// Copyright (C) 2017 Chen-Pang He <chen.pang.he@jdh8.org>
//
// This Source Code Form is subject to the terms of the Mozilla
// Public License v. 2.0. If a copy of the MPL was not distributed
// with this file, You can obtain one at http://mozilla.org/MPL/2.0/

#include <internal/reinterpret>
#include <cmath>
#include <cstdint>

namespace std {
namespace __internal {

static float _kernel(float x)
{
    std::int32_t i = reinterpret<std::int32_t>(x);

    if (i == 0 || i >= 0x7F800000)
        return x;

    if (i < 0x00800000) {
        int shift = __builtin_clz(i) - 8;
        i = (i << shift) - (shift << 23);
    }

    i = 0x2A512CE3 + i / 3;

    double y = reinterpret<float>(i);

    y *= 0.5 + 1.5 * x / (2 * y * (y * y) + x);
    y += 0.33333333333333333333 * (x / (y * y) - y);

    return y;
}

extern "C"
float cbrtf(float x)
{
    return std::copysign(_kernel(std::abs(x)), x);
}

} // namespace __internal
} // namespace std

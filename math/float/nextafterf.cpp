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

extern "C"
float nextafterf(float from, float to)
{
    using std::__internal::reinterpret;

    const std::int32_t mini = 1;

    if (from == to || to != to)
        return to;

    if (from != from)
        return from;

    if (from == 0)
        return std::copysign(reinterpret<float>(mini), to);

    std::int32_t d = reinterpret<std::int32_t>(from);
    std::int32_t a = reinterpret<std::int32_t>(to);

    a < d || (a ^ d) < 0 ? --d : ++d;

    return reinterpret<float>(d);
}

// This file is part of Metallic, a runtime library for WebAssembly.
//
// Copyright (C) 2017 Chen-Pang He <chen.pang.he@jdh8.org>
//
// This Source Code Form is subject to the terms of the Mozilla
// Public License v. 2.0. If a copy of the MPL was not distributed
// with this file, You can obtain one at http://mozilla.org/MPL/2.0/

#ifndef METALLIC_EXPF
#define METALLIC_EXPF

#include "kernel/expm1f"
#include <internal/reinterpret>
#include <cmath>
#include <cstdint>

namespace std {
namespace __internal {

template<typename Scalar>
Scalar expf(Scalar x)
{
    const Scalar minimum = -103.972077083991796;
    const Scalar maximum = 88.7228391116729996;

    const Scalar log2e = 1.442695040888963407;
    const double ln2 = 0.6931471805599453094;

    if (x < minimum)
        return 0;

    if (x > maximum)
        return x * HUGE_VALF;

    Scalar n = std::nearbyint(x * log2e);
    double y = 1 + kernel::expm1f(x - n * ln2);
    int64_t shifted = reinterpret<int64_t>(y) + (static_cast<int64_t>(n) << 52);

    return reinterpret<double>(shifted);
}

} // namespace __internal
} // namespace std

#endif

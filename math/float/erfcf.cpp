// This file is part of Metallic, a runtime library for WebAssembly.
//
// Copyright (C) 2017 Chen-Pang He <chen.pang.he@jdh8.org>
//
// This Source Code Form is subject to the terms of the Mozilla
// Public License v. 2.0. If a copy of the MPL was not distributed
// with this file, You can obtain one at http://mozilla.org/MPL/2.0/

#include "kernel/erfcf"
#include <cmath>

namespace std {
namespace __internal {

extern "C"
float erfcf(float x)
{
    double t = kernel::erfcf(std::abs(x));

    return std::signbit(x) ? 2 - t : t;
}

} // namespace internal
} // namespace std

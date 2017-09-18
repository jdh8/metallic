// This file is part of Metallic, a runtime library for WebAssembly.
//
// Copyright (C) 2017 Chen-Pang He <chen.pang.he@jdh8.org>
//
// This Source Code Form is subject to the terms of the Mozilla
// Public License v. 2.0. If a copy of the MPL was not distributed
// with this file, You can obtain one at http://mozilla.org/MPL/2.0/

#include <type_traits>

static_assert(std::is_integral<int>::value, "False negative");
static_assert(std::is_integral<const bool>::value, "False negative");
static_assert(std::is_integral<volatile unsigned>::value, "False negative");

static_assert(!std::is_integral<float>::value, "False positive");
static_assert(!std::is_integral<const void>::value, "False positive");
static_assert(!std::is_integral<volatile void*>::value, "False positive");

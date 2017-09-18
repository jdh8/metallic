// This file is part of Metallic, a runtime library for WebAssembly.
//
// Copyright (C) 2017 Chen-Pang He <chen.pang.he@jdh8.org>
//
// This Source Code Form is subject to the terms of the Mozilla
// Public License v. 2.0. If a copy of the MPL was not distributed
// with this file, You can obtain one at http://mozilla.org/MPL/2.0/

#include <type_traits>

static_assert(std::is_void<void>::value, "False negative");
static_assert(std::is_void<const void>::value, "False negative");
static_assert(std::is_void<volatile void>::value, "False negative");
static_assert(std::is_void<const volatile void>::value, "False negative");

class Dummy;

static_assert(!std::is_void<const void*>::value, "False positive");
static_assert(!std::is_void<char>::value, "False positive");
static_assert(!std::is_void<Dummy>::value, "False positive");

// This file is part of Metallic, a runtime library for WebAssembly.
//
// Copyright (C) 2017 Chen-Pang He <chen.pang.he@jdh8.org>
//
// This Source Code Form is subject to the terms of the Mozilla
// Public License v. 2.0. If a copy of the MPL was not distributed
// with this file, You can obtain one at http://mozilla.org/MPL/2.0/

#include <type_traits>

typedef int Main();

static_assert(std::is_convertible<int, double>::value, "False negative");
static_assert(std::is_convertible<void, void>::value, "False negative");
static_assert(std::is_convertible<Main*, Main*>::value, "False negative");

static_assert(!std::is_convertible<float[], float[]>::value, "False positive");
static_assert(!std::is_convertible<Main, Main>::value, "False positive");

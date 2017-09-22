// This file is part of Metallic, a runtime library for WebAssembly.
//
// Copyright (C) 2017 Chen-Pang He <chen.pang.he@jdh8.org>
//
// This Source Code Form is subject to the terms of the Mozilla
// Public License v. 2.0. If a copy of the MPL was not distributed
// with this file, You can obtain one at http://mozilla.org/MPL/2.0/

#include <type_traits>

static_assert(std::is_default_constructible<double>::value, "");
static_assert(std::is_default_constructible<const void*>::value, "");

static_assert(!std::is_default_constructible<int&>::value, "");
static_assert(!std::is_default_constructible<int&&>::value, "");

enum List {};

static_assert(std::is_default_constructible<List>::value, "");
static_assert(std::is_default_constructible<List*>::value, "");
static_assert(!std::is_default_constructible<List&>::value, "");

class Stub {};
struct Custom { Custom(); };

class Private { Private(); };
struct Shadowed { Shadowed(Shadowed&&); };
struct Deleted { Deleted() = delete; };

static_assert(std::is_default_constructible<Stub>::value, "");
static_assert(std::is_default_constructible<Custom>::value, "");

static_assert(!std::is_default_constructible<Private>::value, "");
static_assert(!std::is_default_constructible<Shadowed>::value, "");
static_assert(!std::is_default_constructible<Deleted>::value, "");

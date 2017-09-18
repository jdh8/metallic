// This file is part of Metallic, a runtime library for WebAssembly.
//
// Copyright (C) 2017 Chen-Pang He <chen.pang.he@jdh8.org>
//
// This Source Code Form is subject to the terms of the Mozilla
// Public License v. 2.0. If a copy of the MPL was not distributed
// with this file, You can obtain one at http://mozilla.org/MPL/2.0/

#include <type_traits>

static_assert(!std::is_constructible<void>::value, "False positive");
static_assert(!std::is_constructible<int*, int>::value, "False positive");

class Base {};
class Derived : public Base {};

static_assert(std::is_constructible<Base, Derived>::value, "Bug with inheritance");
static_assert(std::is_constructible<Base*, Derived*>::value, "Bug with inheritance");
static_assert(std::is_constructible<Base&, Derived&>::value, "Bug with inheritance");

static_assert(!std::is_constructible<Derived, Base>::value, "Bug with inheritance");
static_assert(!std::is_constructible<Derived*, Base*>::value, "Bug with inheritance");
static_assert(!std::is_constructible<Derived&, Base&>::value, "Bug with inheritance");

// This file is part of Metallic, a runtime library for WebAssembly.
//
// Copyright (C) 2017 Chen-Pang He <chen.pang.he@jdh8.org>
//
// This Source Code Form is subject to the terms of the Mozilla
// Public License v. 2.0. If a copy of the MPL was not distributed
// with this file, You can obtain one at http://mozilla.org/MPL/2.0/

#include <type_traits>

static_assert(!std::is_constructible<void>::value, "`void` shall be unconstructible.");
static_assert(!std::is_constructible<int*, int>::value, "Pointer shall be unconstructible from an integer");

typedef int Main();

static_assert(!std::is_constructible<Main, Main>::value, "Function shall be unconstructible.");
static_assert(std::is_constructible<Main*, Main>::value, "Function pointer shall be constructible.");
static_assert(std::is_constructible<Main&, Main>::value, "Function reference shall be constructible.");

class Base {};
class Derived : public Base {};

static_assert(std::is_constructible<Base, Derived>::value, "Upcasting shall be allowed.");
static_assert(std::is_constructible<Base*, Derived*>::value, "Upcasting shall be allowed.");
static_assert(std::is_constructible<Base&, Derived&>::value, "Upcasting shall be allowed.");

static_assert(!std::is_constructible<Derived, Base>::value, "Downcasting shall be disallowed.");
static_assert(!std::is_constructible<Derived*, Base*>::value, "Downcasting shall be disallowed.");
static_assert(!std::is_constructible<Derived&, Base&>::value, "Downcasting shall be disallowed.");

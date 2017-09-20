// This file is part of Metallic, a runtime library for WebAssembly.
//
// Copyright (C) 2017 Chen-Pang He <chen.pang.he@jdh8.org>
//
// This Source Code Form is subject to the terms of the Mozilla
// Public License v. 2.0. If a copy of the MPL was not distributed
// with this file, You can obtain one at http://mozilla.org/MPL/2.0/

#include <type_traits>

static_assert(!std::is_constructible<void>::value, "");
static_assert(!std::is_constructible<void, void>::value, "");
static_assert(!std::is_constructible<void, int>::value, "");
static_assert(!std::is_constructible<void, int, int>::value, "");

static_assert(std::is_constructible<int, double>::value, "");
static_assert(std::is_constructible<float, double>::value, "");

static_assert(!std::is_constructible<int*, int>::value, "");
static_assert(!std::is_constructible<int, void*>::value, "");

static_assert(std::is_constructible<void*, double*>::value, "");
static_assert(std::is_constructible<const void*, double*>::value, "");
static_assert(!std::is_constructible<double*, void*>::value, "");
static_assert(!std::is_constructible<void*, const double*>::value, "");

static_assert(std::is_constructible<int&&, int>::value, "");
static_assert(std::is_constructible<const int&, const int>::value, "");
static_assert(std::is_constructible<const int&, const int&&>::value, "");

static_assert(!std::is_constructible<const int&>::value, "");
static_assert(!std::is_constructible<int&, int>::value, "");
static_assert(!std::is_constructible<int&, int&&>::value, "");
static_assert(!std::is_constructible<int&&, const int>::value, "");

enum List {};

static_assert(std::is_constructible<List>::value, "Enumerator shall be default-constructible.");
static_assert(std::is_constructible<List, List>::value, "Enumerator shall be copy-constructible.");
static_assert(!std::is_constructible<List, int>::value, "Enumerator construction from integer requires casting.");

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

template<typename Scalar>
struct Pair
{
    Pair(Scalar, Scalar);
};

static_assert(std::is_constructible<Pair<int>, int, int>::value, "User-defined construction");
static_assert(std::is_constructible<Pair<int>, Pair<int>>::value, "Copy construction");
static_assert(!std::is_constructible<Pair<int>>::value, "Deleted default construction");

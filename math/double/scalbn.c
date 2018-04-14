/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2018 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#define SCALBN scalbn

typedef int Integer;

#include "scalbn.h"
#include <limits.h>

double ldexp(double, int) __attribute__((alias("scalbn")));

#if LONG_MAX == INT_MAX
double scalbln(double, long) __attribute__((alias("scalbn")));
#endif

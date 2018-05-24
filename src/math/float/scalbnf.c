/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2018 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#define SCALBNF scalbnf

typedef int Integer;

#include "scalbnf.h"
#include <limits.h>

float ldexpf(float, int) __attribute__((alias("scalbnf")));

#if LONG_MAX == INT_MAX
float scalblnf(float, long) __attribute__((alias("scalbnf")));
#endif

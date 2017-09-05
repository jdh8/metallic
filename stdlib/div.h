/* This file is part of Metallic, a C++ library for WebAssembly.
 *
 * Copyright (C) 2017 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include <stdlib.h>

#define CAT2(a, b) a##b
#define CAT(a, b)  CAT2(a, b)

typedef CAT(PREFIX, div_t) Result;

Result CAT(PREFIX, div)(Integer numerator, Integer denominator)
{
    Result result;

    result.quot = numerator / denominator;
    result.rem = numerator % denominator;

    return result;
}

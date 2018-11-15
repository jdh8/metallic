/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2017 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include <stdint.h>

static uint_fast32_t state = 1;

void srand(unsigned seed)
{
    state = seed;
}

int rand(void)
{
    return state = state * 48271 % 2147483647;
}

int random(void) __attribute__((alias("rand")));

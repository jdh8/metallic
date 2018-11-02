/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2018 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include "../../../src/soft/integer/fixunsdfti.h"
#include "unary.h"

int main(void)
{
    for (int64_t i = 0; i < 0x47F0000000000000; i += 0x000000034E3F771E) {
        double x = reinterpret(double, i);
        verify(_fixunsdfti(x) == (unsigned __int128)x, x);
        verify(!_fixunsdfti(-x), x);
    }
}

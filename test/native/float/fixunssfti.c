/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2018 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include "../../../src/soft/integer/fixunssfti.h"
#include "unary.h"

int main(void)
{
    for (uint32_t i = 0; i < 0x7F800000; i += 71) {
        float x = reinterpret(float, i);
        verify(_fixunssfti(x) == (unsigned __int128)x, x);
        verify(!_fixunssfti(-x), x);
    }
}

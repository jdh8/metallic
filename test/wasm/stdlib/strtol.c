/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2018 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include "../../assert.h"
#include <stdlib.h>
#include <limits.h>

int main(void)
{
    char* dummy;

    metallic_assert(strtol("  -665654654166546565625465265265465", &dummy, 0) == LONG_MIN);
}

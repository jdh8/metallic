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
#include <errno.h>

int main(void)
{
    char* dummy;

    metallic_assert(strtoul("13579", &dummy, 0) == 13579);
    metallic_assert(strtoul("0x6234", &dummy, 0) == 0x6234);
    metallic_assert(strtoul("-0654", &dummy, 0) == -0654);
    metallic_assert(!errno);

    metallic_assert(strtoul("nonsense", &dummy, 0) == 0);

    metallic_assert(strtoul("-321f5a16ga5s1g65as05vs", &dummy, 36) == -1);
    metallic_assert(strtoul("154165asjndiniasndi3sd", &dummy, 36) == -1);
    metallic_assert(errno == ERANGE);
}

/* This file is part of Metallic, a C++ library for WebAssembly.
 *
 * Copyright (C) 2017 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include "bits/specifiers.h"

typedef __SIZE_TYPE__ size_t;

#ifdef __cplusplus
extern "C" {
#endif

_METALLIC_NORETURN void _Exit(int) _METALLIC_NOEXCEPT;
_METALLIC_NORETURN void abort(void) _METALLIC_NOEXCEPT;
_METALLIC_NORETURN void exit(int);

#ifdef __cplusplus
}
#endif

/* This file is part of Metallic, a C++ library for WebAssembly.
 *
 * Copyright (C) 2017 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
typedef __SIZE_TYPE__ size_t;

#ifdef __cplusplus
#define _Noreturn [[noreturn]]
extern "C" {
#endif

_Noreturn inline void abort(void) { __builtin_trap(); }

_Noreturn void exit(int);
_Noreturn void _Exit(int);

#ifdef __cplusplus
}
#undef _Noreturn
#endif

/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2018 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
struct __FILE;

#ifdef __cplusplus
extern "C"
#endif
__SIZE_TYPE__ __stderr(const void*, __SIZE_TYPE__, struct __FILE*);

#define _STDERR(x) __stderr((x), sizeof(x) - 1, 0)
#define _STRINGIFYIMPL(x) #x
#define _STRINGIFY(x) _STRINGIFYIMPL(x)

#ifdef NDEBUG
#define assert(cond) ((void)0)
#else
#define assert(cond) do if (!(cond)) {            \
    _STDERR(__FILE__":"_STRINGIFY(__LINE__)": "); \
    _STDERR(__func__);                            \
    _STDERR(": Assertion `"#cond"' failed.\n");   \
    __builtin_trap();                             \
} while (0)
#endif

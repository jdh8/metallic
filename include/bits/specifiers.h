/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2017 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#ifndef _BITS_SPECIFIERS_H
#define _BITS_SPECIFIERS_H

#if __cplusplus >= 201103L
#define _NORETURN [[noreturn]]
#elif __STDC_VERSION__ >= 201112L
#define _NORETURN _Noreturn
#elif __GNUC__
#define _NORETURN __attribute__((__noreturn__))
#else
#define _NORETURN
#endif

#if __cplusplus >= 201103L
#define _NOEXCEPT noexcept
#else
#define _NOEXCEPT
#endif

#if __STDC_VERSION__ >= 199901L
#define _STATIC static
#define _RESTRICT restrict
#else
#define _STATIC
#define _RESTRICT
#endif

#endif /* bits/specifiers.h */

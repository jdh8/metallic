#ifndef _SETJMP_H
#define _SETJMP_H

#include "bits/specifiers.h"

/* WebAssembly has no native stack-unwinding primitive, so a real
 * jmp_buf would be unused. Keep the type the smallest C requires:
 * one element of an arithmetic type, so the array decays to a
 * pointer just like a real setjmp implementation.
 */
typedef int jmp_buf[1];

#ifdef __cplusplus
extern "C" {
#endif

int setjmp(jmp_buf env);
_NORETURN void longjmp(jmp_buf env, int val);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* setjmp.h */

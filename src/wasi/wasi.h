#ifndef METALLIC_WASI_H
#define METALLIC_WASI_H

#include <stddef.h>
#include <stdint.h>

#if _POSIX_C_SOURCE < 199309L
#undef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 199309L
#endif
#include <time.h>

typedef uint16_t __wasi_errno_t;
typedef uint32_t __wasi_fd_t;
typedef uint64_t __wasi_filesize_t;
typedef int64_t  __wasi_filedelta_t;
typedef uint64_t __wasi_timestamp_t;
typedef uint32_t __wasi_clockid_t;
typedef uint8_t  __wasi_whence_t;
typedef uint32_t __wasi_exitcode_t;

typedef struct {
    const void* buf;
    size_t buf_len;
} __wasi_ciovec_t;

typedef struct {
    void* buf;
    size_t buf_len;
} __wasi_iovec_t;

/* WASI clock ids match POSIX values used by metallic. */
#define __WASI_CLOCKID_REALTIME           ((__wasi_clockid_t)0)
#define __WASI_CLOCKID_MONOTONIC          ((__wasi_clockid_t)1)
#define __WASI_CLOCKID_PROCESS_CPUTIME_ID ((__wasi_clockid_t)2)
#define __WASI_CLOCKID_THREAD_CPUTIME_ID  ((__wasi_clockid_t)3)

/* metallic's <time.h> exposes the first three; CLOCK_THREAD_CPUTIME_ID
 * is not declared so we only assert the ones we use. */
_Static_assert(CLOCK_REALTIME == 0, "CLOCK_REALTIME id mismatch");
_Static_assert(CLOCK_MONOTONIC == 1, "CLOCK_MONOTONIC id mismatch");
_Static_assert(CLOCK_PROCESS_CPUTIME_ID == 2, "CLOCK_PROCESS_CPUTIME_ID mismatch");

/* WASI whence: 0=SET, 1=CUR, 2=END.  metallic's <stdio.h> SEEK_* match. */

#define WASI_IMPORT(name) \
    __attribute__((import_module("wasi_snapshot_preview1"), import_name(#name)))

WASI_IMPORT(fd_read)
__wasi_errno_t __wasi_fd_read(__wasi_fd_t fd, const __wasi_iovec_t* iovs, size_t iovs_len, size_t* nread);

WASI_IMPORT(fd_write)
__wasi_errno_t __wasi_fd_write(__wasi_fd_t fd, const __wasi_ciovec_t* iovs, size_t iovs_len, size_t* nwritten);

WASI_IMPORT(fd_seek)
__wasi_errno_t __wasi_fd_seek(__wasi_fd_t fd, __wasi_filedelta_t offset, __wasi_whence_t whence, __wasi_filesize_t* new_offset);

WASI_IMPORT(fd_close)
__wasi_errno_t __wasi_fd_close(__wasi_fd_t fd);

WASI_IMPORT(clock_time_get)
__wasi_errno_t __wasi_clock_time_get(__wasi_clockid_t id, __wasi_timestamp_t precision, __wasi_timestamp_t* time);

WASI_IMPORT(proc_exit)
_Noreturn void __wasi_proc_exit(__wasi_exitcode_t rc);

#endif /* METALLIC_WASI_H */

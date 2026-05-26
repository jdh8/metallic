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

/* ---------------------------------------------------------------- *
 *  File I/O (preopen-based path resolution)                        *
 * ---------------------------------------------------------------- */

typedef uint32_t __wasi_lookupflags_t;
typedef uint16_t __wasi_oflags_t;
typedef uint64_t __wasi_rights_t;
typedef uint16_t __wasi_fdflags_t;
typedef uint8_t  __wasi_filetype_t;

#define __WASI_LOOKUPFLAGS_SYMLINK_FOLLOW ((__wasi_lookupflags_t)1)

#define __WASI_OFLAGS_CREAT     ((__wasi_oflags_t)1)
#define __WASI_OFLAGS_DIRECTORY ((__wasi_oflags_t)2)
#define __WASI_OFLAGS_EXCL      ((__wasi_oflags_t)4)
#define __WASI_OFLAGS_TRUNC     ((__wasi_oflags_t)8)

#define __WASI_FDFLAGS_APPEND   ((__wasi_fdflags_t)1)
#define __WASI_FDFLAGS_DSYNC    ((__wasi_fdflags_t)2)
#define __WASI_FDFLAGS_NONBLOCK ((__wasi_fdflags_t)4)
#define __WASI_FDFLAGS_RSYNC    ((__wasi_fdflags_t)8)
#define __WASI_FDFLAGS_SYNC     ((__wasi_fdflags_t)16)

#define __WASI_RIGHTS_FD_DATASYNC           ((__wasi_rights_t)1ULL << 0)
#define __WASI_RIGHTS_FD_READ               ((__wasi_rights_t)1ULL << 1)
#define __WASI_RIGHTS_FD_SEEK               ((__wasi_rights_t)1ULL << 2)
#define __WASI_RIGHTS_FD_FDSTAT_SET_FLAGS   ((__wasi_rights_t)1ULL << 3)
#define __WASI_RIGHTS_FD_SYNC               ((__wasi_rights_t)1ULL << 4)
#define __WASI_RIGHTS_FD_TELL               ((__wasi_rights_t)1ULL << 5)
#define __WASI_RIGHTS_FD_WRITE              ((__wasi_rights_t)1ULL << 6)
#define __WASI_RIGHTS_FD_FILESTAT_GET       ((__wasi_rights_t)1ULL << 21)
#define __WASI_RIGHTS_FD_FILESTAT_SET_SIZE  ((__wasi_rights_t)1ULL << 22)
#define __WASI_RIGHTS_PATH_CREATE_FILE      ((__wasi_rights_t)1ULL << 9)
#define __WASI_RIGHTS_PATH_OPEN             ((__wasi_rights_t)1ULL << 13)
#define __WASI_RIGHTS_PATH_FILESTAT_GET     ((__wasi_rights_t)1ULL << 18)
#define __WASI_RIGHTS_PATH_UNLINK_FILE      ((__wasi_rights_t)1ULL << 26)
#define __WASI_RIGHTS_PATH_REMOVE_DIRECTORY ((__wasi_rights_t)1ULL << 17)
#define __WASI_RIGHTS_PATH_RENAME_SOURCE    ((__wasi_rights_t)1ULL << 15)
#define __WASI_RIGHTS_PATH_RENAME_TARGET    ((__wasi_rights_t)1ULL << 16)
#define __WASI_RIGHTS_FD_READDIR            ((__wasi_rights_t)1ULL << 14)

#define __WASI_FILETYPE_UNKNOWN          ((__wasi_filetype_t)0)
#define __WASI_FILETYPE_BLOCK_DEVICE     ((__wasi_filetype_t)1)
#define __WASI_FILETYPE_CHARACTER_DEVICE ((__wasi_filetype_t)2)
#define __WASI_FILETYPE_DIRECTORY        ((__wasi_filetype_t)3)
#define __WASI_FILETYPE_REGULAR_FILE     ((__wasi_filetype_t)4)
#define __WASI_FILETYPE_SOCKET_DGRAM     ((__wasi_filetype_t)5)
#define __WASI_FILETYPE_SOCKET_STREAM    ((__wasi_filetype_t)6)
#define __WASI_FILETYPE_SYMBOLIC_LINK    ((__wasi_filetype_t)7)

typedef struct {
    uint64_t dev;
    uint64_t ino;
    __wasi_filetype_t filetype;
    uint64_t nlink;
    uint64_t size;
    uint64_t atim;
    uint64_t mtim;
    uint64_t ctim;
} __wasi_filestat_t;

typedef struct {
    __wasi_filetype_t fs_filetype;
    __wasi_fdflags_t  fs_flags;
    __wasi_rights_t   fs_rights_base;
    __wasi_rights_t   fs_rights_inheriting;
} __wasi_fdstat_t;

/* Prestat = preopen status. The "u" union has a "dir" arm with a name length. */
typedef struct {
    uint8_t tag; /* 0 = dir */
    union { struct { size_t pr_name_len; } dir; } u;
} __wasi_prestat_t;

#define __WASI_ERRNO_BADF ((__wasi_errno_t)8)

WASI_IMPORT(fd_prestat_get)
__wasi_errno_t __wasi_fd_prestat_get(__wasi_fd_t fd, __wasi_prestat_t* buf);

WASI_IMPORT(fd_prestat_dir_name)
__wasi_errno_t __wasi_fd_prestat_dir_name(__wasi_fd_t fd, uint8_t* path, size_t path_len);

WASI_IMPORT(path_open)
__wasi_errno_t __wasi_path_open(__wasi_fd_t base, __wasi_lookupflags_t lf,
                                const char* path, size_t path_len,
                                __wasi_oflags_t oflags,
                                __wasi_rights_t fs_rights_base,
                                __wasi_rights_t fs_rights_inheriting,
                                __wasi_fdflags_t fdflags,
                                __wasi_fd_t* new_fd);

WASI_IMPORT(path_unlink_file)
__wasi_errno_t __wasi_path_unlink_file(__wasi_fd_t base, const char* path, size_t path_len);

WASI_IMPORT(path_remove_directory)
__wasi_errno_t __wasi_path_remove_directory(__wasi_fd_t base, const char* path, size_t path_len);

WASI_IMPORT(path_rename)
__wasi_errno_t __wasi_path_rename(__wasi_fd_t old_fd, const char* old_path, size_t old_path_len,
                                  __wasi_fd_t new_fd, const char* new_path, size_t new_path_len);

WASI_IMPORT(path_filestat_get)
__wasi_errno_t __wasi_path_filestat_get(__wasi_fd_t base, __wasi_lookupflags_t lf,
                                        const char* path, size_t path_len,
                                        __wasi_filestat_t* buf);

WASI_IMPORT(fd_filestat_get)
__wasi_errno_t __wasi_fd_filestat_get(__wasi_fd_t fd, __wasi_filestat_t* buf);

WASI_IMPORT(fd_fdstat_get)
__wasi_errno_t __wasi_fd_fdstat_get(__wasi_fd_t fd, __wasi_fdstat_t* buf);

WASI_IMPORT(fd_fdstat_set_flags)
__wasi_errno_t __wasi_fd_fdstat_set_flags(__wasi_fd_t fd, __wasi_fdflags_t flags);

#endif /* METALLIC_WASI_H */

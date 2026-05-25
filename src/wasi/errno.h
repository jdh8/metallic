#ifndef METALLIC_WASI_ERRNO_H
#define METALLIC_WASI_ERRNO_H

#include <errno.h>
#include "wasi.h"

/* Translation from WASI snapshot_preview1 errno (indices 0..76) to
 * metallic's POSIX errno numbering (see include/errno.h).
 *
 * Substitutions noted:
 *   ENOTCAPABLE (76)  -> EACCES   (no POSIX counterpart in metallic)
 *
 * All other WASI errnos have direct counterparts in metallic's errno.h,
 * including EMULTIHOP, EOWNERDEAD, and ENOTRECOVERABLE.  ENOTSUP maps
 * to metallic's EOPNOTSUPP (POSIX permits them to share a value). */
static const int wasi_to_posix[77] = {
    0,                /*  0 SUCCESS         */
    E2BIG,            /*  1 E2BIG           */
    EACCES,           /*  2 EACCES          */
    EADDRINUSE,       /*  3 EADDRINUSE      */
    EADDRNOTAVAIL,    /*  4 EADDRNOTAVAIL   */
    EAFNOSUPPORT,     /*  5 EAFNOSUPPORT    */
    EAGAIN,           /*  6 EAGAIN          */
    EALREADY,         /*  7 EALREADY        */
    EBADF,            /*  8 EBADF           */
    EBADMSG,          /*  9 EBADMSG         */
    EBUSY,            /* 10 EBUSY           */
    ECANCELED,        /* 11 ECANCELED       */
    ECHILD,           /* 12 ECHILD          */
    ECONNABORTED,     /* 13 ECONNABORTED    */
    ECONNREFUSED,     /* 14 ECONNREFUSED    */
    ECONNRESET,       /* 15 ECONNRESET      */
    EDEADLK,          /* 16 EDEADLK         */
    EDESTADDRREQ,     /* 17 EDESTADDRREQ    */
    EDOM,             /* 18 EDOM            */
    EDQUOT,           /* 19 EDQUOT          */
    EEXIST,           /* 20 EEXIST          */
    EFAULT,           /* 21 EFAULT          */
    EFBIG,            /* 22 EFBIG           */
    EHOSTUNREACH,     /* 23 EHOSTUNREACH    */
    EIDRM,            /* 24 EIDRM           */
    EILSEQ,           /* 25 EILSEQ          */
    EINPROGRESS,      /* 26 EINPROGRESS     */
    EINTR,            /* 27 EINTR           */
    EINVAL,           /* 28 EINVAL          */
    EIO,              /* 29 EIO             */
    EISCONN,          /* 30 EISCONN         */
    EISDIR,           /* 31 EISDIR          */
    ELOOP,            /* 32 ELOOP           */
    EMFILE,           /* 33 EMFILE          */
    EMLINK,           /* 34 EMLINK          */
    EMSGSIZE,         /* 35 EMSGSIZE        */
    EMULTIHOP,        /* 36 EMULTIHOP       */
    ENAMETOOLONG,     /* 37 ENAMETOOLONG    */
    ENETDOWN,         /* 38 ENETDOWN        */
    ENETRESET,        /* 39 ENETRESET       */
    ENETUNREACH,      /* 40 ENETUNREACH     */
    ENFILE,           /* 41 ENFILE          */
    ENOBUFS,          /* 42 ENOBUFS         */
    ENODEV,           /* 43 ENODEV          */
    ENOENT,           /* 44 ENOENT          */
    ENOEXEC,          /* 45 ENOEXEC         */
    ENOLCK,           /* 46 ENOLCK          */
    ENOLINK,          /* 47 ENOLINK         */
    ENOMEM,           /* 48 ENOMEM          */
    ENOMSG,           /* 49 ENOMSG          */
    ENOPROTOOPT,      /* 50 ENOPROTOOPT     */
    ENOSPC,           /* 51 ENOSPC          */
    ENOSYS,           /* 52 ENOSYS          */
    ENOTCONN,         /* 53 ENOTCONN        */
    ENOTDIR,          /* 54 ENOTDIR         */
    ENOTEMPTY,        /* 55 ENOTEMPTY       */
    ENOTRECOVERABLE,  /* 56 ENOTRECOVERABLE */
    ENOTSOCK,         /* 57 ENOTSOCK        */
    EOPNOTSUPP,       /* 58 ENOTSUP (== EOPNOTSUPP) */
    ENOTTY,           /* 59 ENOTTY          */
    ENXIO,            /* 60 ENXIO           */
    EOVERFLOW,        /* 61 EOVERFLOW       */
    EOWNERDEAD,       /* 62 EOWNERDEAD      */
    EPERM,            /* 63 EPERM           */
    EPIPE,            /* 64 EPIPE           */
    EPROTO,           /* 65 EPROTO          */
    EPROTONOSUPPORT,  /* 66 EPROTONOSUPPORT */
    EPROTOTYPE,       /* 67 EPROTOTYPE      */
    ERANGE,           /* 68 ERANGE          */
    EROFS,            /* 69 EROFS           */
    ESPIPE,           /* 70 ESPIPE          */
    ESRCH,            /* 71 ESRCH           */
    ESTALE,           /* 72 ESTALE          */
    ETIMEDOUT,        /* 73 ETIMEDOUT       */
    ETXTBSY,          /* 74 ETXTBSY         */
    EXDEV,            /* 75 EXDEV           */
    EACCES,           /* 76 ENOTCAPABLE -> EACCES (substitute) */
};

static inline long wasi_check(__wasi_errno_t e)
{
    return e ? -wasi_to_posix[e] : 0;
}

#endif /* METALLIC_WASI_ERRNO_H */

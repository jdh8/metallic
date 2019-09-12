#ifndef	_FCNTL_H
#define	_FCNTL_H

typedef unsigned mode_t;
typedef long long off_t;
typedef int pid_t;

struct flock
{
    short l_type;
    short l_whence;
    off_t l_start;
    off_t l_len;
    pid_t l_pid;
};

#define F_DUPFD  0
#define F_GETFD  1
#define F_SETFD  2
#define F_GETFL  3
#define F_SETFL  4

#define F_GETLK  5
#define F_SETLK  6
#define F_SETLKW 7

#define F_SETOWN 8
#define F_GETOWN 9

#define FD_CLOEXEC 1

#define F_RDLCK 0
#define F_WRLCK 1
#define F_UNLCK 2

#define O_CREAT        0100
#define O_EXCL         0200
#define O_NOCTTY       0400
#define O_TRUNC       01000
#define O_APPEND      02000
#define O_NONBLOCK    04000
#define O_DSYNC      010000
#define O_SYNC     04010000
#define O_RSYNC    04010000
#define O_DIRECTORY 0200000
#define O_NOFOLLOW  0400000
#define O_CLOEXEC  02000000

#define O_ASYNC      020000
#define O_DIRECT     040000
#define O_LARGEFILE       0
#define O_NOATIME  01000000
#define O_PATH    010000000
#define O_TMPFILE 020200000
#define O_NDELAY O_NONBLOCK

#define O_SEARCH   O_PATH
#define O_EXEC     O_PATH
#define O_TTY_INIT 0

#define O_ACCMODE (3 | O_EXEC)
#define O_RDONLY  0
#define O_WRONLY  1
#define O_RDWR    2

#define POSIX_FADV_NORMAL     0
#define POSIX_FADV_RANDOM     1
#define POSIX_FADV_SEQUENTIAL 2
#define POSIX_FADV_WILLNEED   3
#define POSIX_FADV_DONTNEED   4
#define POSIX_FADV_NOREUSE    5

#ifdef __cplusplus
extern "C" {
#endif

int creat(const char*, mode_t);
int fcntl(int, int, ...);
int open(const char*, int, ...);

int posix_fadvise(int, off_t, off_t, int);
int posix_fallocate(int, off_t, off_t);

#ifdef __cplusplus
}
#endif
#endif /* fcntl.h */

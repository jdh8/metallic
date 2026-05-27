#include <string.h>
#include <errno.h>

char* strerror(int e)
{
    switch (e) {
        case 0:               return "Success";
        case EPERM:           return "Operation not permitted";
        case ENOENT:          return "No such file or directory";
        case ESRCH:           return "No such process";
        case EINTR:           return "Interrupted system call";
        case EIO:             return "I/O error";
        case ENXIO:           return "No such device or address";
        case E2BIG:           return "Argument list too long";
        case ENOEXEC:         return "Exec format error";
        case EBADF:           return "Bad file descriptor";
        case ECHILD:          return "No child processes";
        case EAGAIN:          return "Resource temporarily unavailable";
        case ENOMEM:          return "Out of memory";
        case EACCES:          return "Permission denied";
        case EFAULT:          return "Bad address";
        case EBUSY:           return "Device or resource busy";
        case EEXIST:          return "File exists";
        case EXDEV:           return "Cross-device link";
        case ENODEV:          return "No such device";
        case ENOTDIR:         return "Not a directory";
        case EISDIR:          return "Is a directory";
        case EINVAL:          return "Invalid argument";
        case ENFILE:          return "Too many open files in system";
        case EMFILE:          return "Too many open files";
        case ENOTTY:          return "Not a terminal";
        case ETXTBSY:         return "Text file busy";
        case EFBIG:           return "File too large";
        case ENOSPC:          return "No space left on device";
        case ESPIPE:          return "Illegal seek";
        case EROFS:           return "Read-only file system";
        case EMLINK:          return "Too many links";
        case EPIPE:           return "Broken pipe";
        case EDOM:            return "Numerical argument out of domain";
        case ERANGE:          return "Numerical result out of range";
        case EDEADLK:         return "Resource deadlock avoided";
        case ENAMETOOLONG:    return "File name too long";
        case ENOLCK:          return "No locks available";
        case ENOSYS:          return "Function not implemented";
        case ENOTEMPTY:       return "Directory not empty";
        case ELOOP:           return "Too many levels of symbolic links";
        case EILSEQ:          return "Invalid or incomplete multibyte sequence";
        case EOVERFLOW:       return "Value too large for defined data type";
        case ENOTSOCK:        return "Socket operation on non-socket";
        case EMSGSIZE:        return "Message too long";
        case ENOTCONN:        return "Transport endpoint is not connected";
        case ETIMEDOUT:       return "Connection timed out";
        case ECONNREFUSED:    return "Connection refused";
        case EALREADY:        return "Operation already in progress";
        case EINPROGRESS:     return "Operation now in progress";
        case ECANCELED:       return "Operation canceled";
        default:              return "Unknown error";
    }
}

#include <fcntl.h>

static int modeflags_(const char mode[static 1])
{
    int access = O_RDONLY;
    int modifier = 0;

    switch (*mode) {
        case 'w':
            modifier = O_CREAT | O_TRUNC;
            goto writing;
        case 'a':
            modifier = O_CREAT | O_APPEND;
        writing:
            access = O_WRONLY;
        case 'r':
            ++mode;
    }

    for (; *mode; ++mode) {
        switch (*mode) {
            case '+':
                access = O_RDWR;
                break;
            case 'x':
                modifier |= O_EXCL;
                break;
            case 'e':
                modifier |= O_CLOEXEC;
        }
    }

    return access | modifier;
}

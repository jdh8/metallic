#include "../conversion/Memory.h"
#include "../conversion/ToInt32.h"
#include "../conversion/ToInteger.h"
#include <node_api.h>

#ifndef _WIN32
#include <fcntl.h>
#include <errno.h>

struct metallic_flock
{
    int16_t l_type;
    int16_t l_whence;
    int64_t l_start;
    int64_t l_len;
    int32_t l_pid;
};

struct flock convert_(struct metallic_flock arg)
{
    return (struct flock){
        .l_type   = arg.l_type,
        .l_whence = arg.l_whence,
        .l_start  = arg.l_start,
        .l_len    = arg.l_len,
        .l_pid    = arg.l_pid,   
    };
};

static int fcntl_(void* buffer, int fd, int cmd, long arg)
{
    switch (cmd) {
        case F_SETLK:
        case F_SETLKW:
        case F_GETLK:
            return fcntl(fd, cmd, convert_(*(struct metallic_flock*)DataView(buffer, arg)));
        default:
            return fcntl(fd, cmd, (int)arg);
    }
}

napi_value metallic_fcntl(napi_env env, napi_callback_info args)
{
    const size_t arity = 4;
    size_t argc = arity;
    napi_value argv[arity];

    napi_get_cb_info(env, args, &argc, argv, (void*)0, (void*)0);

    napi_value result = 0;
    void* buffer = Memory(env, argv[0]);
    int fd = ToInt32(env, argv[1]);
    int cmd = ToInt32(env, argv[2]);
    long arg = ToInteger(env, argv[3]);
    int status = fcntl_(buffer, fd, cmd, arg);

    napi_create_int32(env, status == -1 ? errno : status, &result);

    return result;
}
#endif

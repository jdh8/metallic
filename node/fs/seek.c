#define NAPI_EXPERIMENTAL
#include "../conversion/Memory.h"
#include "../conversion/ToBigInt64.h"
#include "../conversion/ToInt32.h"
#include "../conversion/ToUint32.h"
#include <node_api.h>
#include <errno.h>

#ifdef _WIN32
#include <io.h>
#define SEEK(fd, offset, origin) _lseeki64(fd, offset, origin)
#else
#include <unistd.h>
#define SEEK(fd, offset, origin) lseek(fd, offset, origin)
#endif

napi_value metallic_lseek(napi_env env, napi_callback_info args)
{
    const size_t arity = 4;
    size_t argc = arity;
    napi_value argv[arity];

    napi_get_cb_info(env, args, &argc, argv, (void*)0, (void*)0);

    napi_value result = 0;
    off_t position = SEEK(ToInt32(env, argv[1]), ToBigInt64(env, argv[2]), ToInt32(env, argv[3]));

    napi_create_bigint_int64(env, position == -1 ? -errno : position, &result);

    return result;
}

napi_value metallic_llseek(napi_env env, napi_callback_info args)
{
    const size_t arity = 6;
    size_t argc = arity;
    napi_value argv[arity];

    napi_get_cb_info(env, args, &argc, argv, (void*)0, (void*)0);

    napi_value status = 0;
    int fd = ToInt32(env, argv[1]);
    off_t offset = (int64_t)ToInt32(env, argv[2]) << 32 | ToUint32(env, argv[3]);
    int64_t* result = DataView(Memory(env, argv[0]), ToInt32(env, argv[4]));
    int origin = ToInt32(env, argv[5]);

    off_t position = SEEK(fd, offset, origin);

    if (position >= 0) {
        napi_create_int32(env, 0, &status);
        *result = position;
    }
    else {
        napi_create_int32(env, -errno, &status);
    }
    return status;
}

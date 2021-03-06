#include "../conversion/ToBigInt64.h"
#include "../conversion/ToInt32.h"
#include "../conversion/ToUint32.h"
#include <node_api.h>

#ifndef _WIN32
#include <fcntl.h>

napi_value metallic_fallocate64(napi_env env, napi_callback_info args)
{
    const size_t arity = 4;
    size_t argc = arity;
    napi_value argv[arity];

    napi_get_cb_info(env, args, &argc, argv, (void*)0, (void*)0);

    napi_value result = 0;
    int status = posix_fallocate(ToInt32(env, argv[1]), ToBigInt64(env, argv[2]), ToBigInt64(env, argv[3]));

    napi_create_bigint_int64(env, -status, &result);

    return result;
}

napi_value metallic_fallocate32(napi_env env, napi_callback_info args)
{
    const size_t arity = 6;
    size_t argc = arity;
    napi_value argv[arity];

    napi_get_cb_info(env, args, &argc, argv, (void*)0, (void*)0);

    napi_value result = 0;
    int fd = ToInt32(env, argv[1]);
    off_t offset = (int64_t)ToInt32(env, argv[2]) << 32 | ToUint32(env, argv[3]);
    off_t length = (int64_t)ToInt32(env, argv[4]) << 32 | ToUint32(env, argv[5]);

    napi_create_int32(env, -posix_fallocate(fd, offset, length), &result);

    return result;
}
#endif

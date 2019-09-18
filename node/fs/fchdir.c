#include "../conversion/ToInt32.h"
#include <node_api.h>

#ifndef _WIN32
#include <unistd.h>
#include <errno.h>

napi_value metallic_fchdir(napi_env env, napi_callback_info args)
{
    const size_t arity = 2;
    size_t argc = arity;
    napi_value argv[arity];

    napi_get_cb_info(env, args, &argc, argv, (void*)0, (void*)0);

    napi_value result = 0;

    napi_create_int32(env, fchdir(ToInt32(env, argv[1])) * errno, &result);

    return result;
}
#endif


#include <node_api.h>

static napi_value Function(napi_env env, napi_callback callback)
{
    napi_value result = 0;

    napi_create_function(env, (void*)0, 0, callback, (void*)0, &result);

    return result;
}

#ifndef _WIN32
napi_value metallic_fallocate32(napi_env, napi_callback_info);
napi_value metallic_fallocate64(napi_env, napi_callback_info);
napi_value metallic_fchdir(napi_env, napi_callback_info);
#endif

napi_value metallic_lseek(napi_env, napi_callback_info);
napi_value metallic_llseek(napi_env, napi_callback_info);

static napi_value constructor(napi_env env, napi_value exports)
{
#ifndef _WIN32
    napi_set_named_property(env, exports, "fallocate32", Function(env, metallic_fallocate32));
    napi_set_named_property(env, exports, "fallocate64", Function(env, metallic_fallocate64));
    napi_set_named_property(env, exports, "fchdir", Function(env, metallic_fchdir));
#endif

    napi_set_named_property(env, exports, "lseek", Function(env, metallic_lseek));
    napi_set_named_property(env, exports, "llseek", Function(env, metallic_llseek));

    return exports;
}

NAPI_MODULE(NODE_GYP_MODULE_NAME, constructor)

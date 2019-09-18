#include <node_api.h>

static napi_value Function(napi_env env, napi_callback callback)
{
    napi_value result = 0;

    napi_create_function(env, (void*)0, 0, callback, (void*)0, &result);

    return result;
}

napi_value metallic_lseek(napi_env, napi_callback_info);
napi_value metallic_llseek(napi_env, napi_callback_info);

static napi_value constructor(napi_env env, napi_value exports)
{
    napi_set_named_property(env, exports, "llseek", Function(env, metallic_lseek));
    napi_set_named_property(env, exports, "llseek", Function(env, metallic_llseek));

    return exports;
}

NAPI_MODULE(NODE_GYP_MODULE_NAME, constructor)

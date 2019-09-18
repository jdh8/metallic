#ifndef METALLIC_NODE_TOINT32_H
#define METALLIC_NODE_TOINT32_H

#include <node_api.h>

static int32_t ToInt32(napi_env env, napi_value value)
{
    int32_t result = 0;

    napi_get_value_int32(env, value, &result);

    return result;
}

#endif

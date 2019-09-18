#ifndef METALLIC_NODE_TOUINT32_H
#define METALLIC_NODE_TOUINT32_H

#include <node_api.h>

static uint32_t ToUint32(napi_env env, napi_value value)
{
    uint32_t result = 0;

    napi_get_value_uint32(env, value, &result);

    return result;
}

#endif

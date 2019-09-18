#ifndef METALLIC_NODE_TOUINT32_H
#define METALLIC_NODE_TOUINT32_H

#include <node_api.h>

static uint32_t ToUint32(napi_env env, napi_value value)
{
    uint32_t result = 0;

    if (napi_get_value_uint32(env, value, &result) == napi_number_expected)
        napi_throw_type_error(env, (void*)0, "Number is expected for uint32_t");

    return result;
}

#endif

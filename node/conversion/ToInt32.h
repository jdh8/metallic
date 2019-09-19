#ifndef METALLIC_NODE_TOINT32_H
#define METALLIC_NODE_TOINT32_H

#include <node_api.h>

static int32_t ToInt32(napi_env env, napi_value value)
{
    int32_t result;

    if (napi_get_value_int32(env, value, &result) == napi_number_expected)
        napi_throw_type_error(env, NULL, "Number is expected for int32_t");

    return result;
}

#endif

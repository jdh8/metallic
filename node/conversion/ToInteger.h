#ifndef METALLIC_NODE_TOINTEGER_H
#define METALLIC_NODE_TOINTEGER_H

#ifndef NAPI_EXPERIMENTAL
#error NAPI_EXPERIMENTAL must be defined before including this header
#endif

#include <node_api.h>

static int64_t ToInteger(napi_env env, napi_value value)
{
    int64_t result;
    bool lossless;

    if (napi_get_value_int64(env, value, &result) == napi_number_expected)
        if (napi_get_value_bigint_int64(env, value, &result, &lossless) == napi_bigint_expected)
            napi_throw_type_error(env, NULL, "Number or BigInt is expected for ToInteger");

    return result;
}

#endif

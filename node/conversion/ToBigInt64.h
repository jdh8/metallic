#ifndef METALLIC_NODE_TOBIGINT64_H
#define METALLIC_NODE_TOBIGINT64_H

#ifndef NAPI_EXPERIMENTAL
#error NAPI_EXPERIMENTAL must be defined before including this header
#endif

#include <node_api.h>

static int64_t ToBigInt64(napi_env env, napi_value value)
{
    int64_t result;
    bool lossless;

    if (napi_get_value_bigint_int64(env, value, &result, &lossless) == napi_bigint_expected)
        napi_throw_type_error(env, NULL, "BigInt is expected for int64_t");

    return result;
}

#endif

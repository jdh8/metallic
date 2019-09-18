#ifndef METALLIC_NODE_TOBIGINT64_H
#define METALLIC_NODE_TOBIGINT64_H

#ifndef NAPI_EXPERIMENTAL
#error NAPI_EXPERIMENTAL must be defined before including this header
#endif

#include <node_api.h>

static int64_t ToBigInt64(napi_env env, napi_value value)
{
    int64_t result = 0;

    if (napi_get_value_bigint_int64(env, value, &result, false) == napi_bigint_expected) {
        napi_throw_type_error(env, (void*)0, "BigInt is expected for int64_t");
        napi_get_value_int64(env, value, &result);
    }
    return result;
}

#endif

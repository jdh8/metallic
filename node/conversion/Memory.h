#ifndef METALLIC_NODE_MEMORY_H
#define METALLIC_NODE_MEMORY_H

#include <node_api.h>

static void* Memory(napi_env env, napi_value memory)
{
    napi_value buffer = 0;
    void* result = 0;

    napi_get_named_property(env, memory, "buffer", &buffer);
    napi_get_arraybuffer_info(env, buffer, &result, (void*)0);

    return result;
}

static void* DataView(void* buffer, ptrdiff_t pointer)
{
    return (unsigned char*)buffer + pointer;
}

#endif

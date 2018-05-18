inline void metallic_assert(_Bool condition)
{
    if (!condition)
        __builtin_trap();
}

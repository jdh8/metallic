static int _clzti2(unsigned __int128 x)
{
    return x >> 64 ? __builtin_clzll(x >> 64) : 64 | __builtin_clzll(x);
}


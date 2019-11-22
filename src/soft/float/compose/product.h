static unsigned __int128 compose_product_(int exp, unsigned __int128 significand)
{
    if (exp >= 0x7FFF)
        return (unsigned __int128)0x7FFF << 112;

    if (exp > 0) {
        unsigned __int128 tail = significand << 1;
        return ((unsigned __int128)exp << 112 | tail >> 16) + (((tail & 0xFFFF) | (tail >> 15 & 1)) > 0x8000);
    }

    if (exp >= -112) {
        unsigned __int128 q = significand >> 16 >> -exp;
        unsigned __int128 r = significand << (112 + exp);
        return q + ((r >> 64 | (q & 1 || r & 0xFFFFFFFFFFFFFFFF)) > 0x8000000000000000);
    }

    return 0;
}

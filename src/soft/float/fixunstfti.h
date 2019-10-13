static unsigned __int128 fixunstfti_(__int128 bits)
{
    const unsigned __int128 implied = (unsigned __int128)1 << 112;

    if (bits >> 64 < 0x3FFF000000000000)
        return 0;

    int shift = 112 + 0x3FFF - (int)(bits >> 112);
    unsigned __int128 significand = (bits & (implied - 1)) | implied;

    return shift < 0 ? significand << -shift : significand >> shift;
}

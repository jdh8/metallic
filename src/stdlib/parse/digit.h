static unsigned digit_(unsigned c)
{
    if (c - '0' < 10)
        return c - '0';

    c |= 32;

    if (c - 'a' < 26)
        return c - 'a' + 10;

    return -1;
}

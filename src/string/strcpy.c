char* strcpy(char destination[static restrict 1], const char source[static restrict 1])
{
    char* output = destination;

    while ((*output++ = *source++));

    return destination;
}

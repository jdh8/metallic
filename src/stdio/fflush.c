#include <stdio.h>

/* metallic's stdio is currently unbuffered: every fwrite/fread goes
 * straight through __stdio_write/__stdio_read to the host. Until a
 * real buffer pool is introduced, fflush has nothing to do. */
int fflush(FILE* stream)
{
    (void)stream;
    return 0;
}

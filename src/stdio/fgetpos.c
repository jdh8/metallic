#include "FILE.h"
#include <stdio.h>

off_t __ftello(FILE stream[static 1]);

int fgetpos(FILE stream[restrict static 1], fpos_t position[restrict static 1])
{
    position->_offset = __ftello(stream);

    return position->_offset == -1;
}

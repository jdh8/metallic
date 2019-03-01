
/* This function shall always fail because there is no shell.
 * Queried with NULL, this function returns 0.
 * With a command, this function returns EXIT_FAILURE.
 */
int system(const char* command) { return !!command; }

#include <assert.h>
#include <string.h>

static size_t (*volatile spn_)(const char*, const char*) = strspn;
static size_t (*volatile cspn_)(const char*, const char*) = strcspn;
static char* (*volatile pbrk_)(const char*, const char*) = strpbrk;

int main(void)
{
    /* Empty set */
    assert(spn_("abc", "") == 0);
    assert(cspn_("abc", "") == 3);
    assert(pbrk_("abc", "") == 0);

    /* Single-character set */
    assert(spn_("aaab", "a") == 3);
    assert(cspn_("bbba", "a") == 3);

    /* Span covering the whole string */
    assert(spn_("abcabc", "cba") == 6);
    assert(cspn_("abcabc", "xyz") == 6);

    /* Empty string */
    assert(spn_("", "abc") == 0);
    assert(cspn_("", "abc") == 0);
    assert(pbrk_("", "abc") == 0);

    /* Bytes >= 0x80: unsigned-char indexing */
    assert(spn_("\xC3\xA9x", "\xC3\xA9") == 2);
    assert(cspn_("ab\xFF" "cd", "\xFF") == 2);
    assert(pbrk_("ab\x80", "\x80")[0] == '\x80');

    /* strpbrk hit and miss */
    assert(pbrk_("hello, world", " ,") == strchr("hello, world", ','));
    assert(pbrk_("hello", "xyz") == 0);

    /* strtok composes strspn + strcspn */
    char text[] = ", one two,three ,";
    assert(strcmp(strtok(text, " ,"), "one") == 0);
    assert(strcmp(strtok(0, " ,"), "two") == 0);
    assert(strcmp(strtok(0, " ,"), "three") == 0);
    assert(strtok(0, " ,") == 0);
}

#ifndef METALLIC_SRC_WCHAR_PARSE_H
#define METALLIC_SRC_WCHAR_PARSE_H

#include <wchar.h>
#include <stddef.h>

/* Transcribe a leading prefix of `s` to a fixed ASCII buffer of size `cap`
 * (including space for the trailing NUL), copying only characters that can
 * appear in C number tokens: ASCII whitespace, sign, digits, hex/exponent
 * letters and indicators for inf/nan.  Stops at the first wide character
 * that maps to no ASCII number-token byte, or when the buffer is one shy of
 * full.  Returns the number of ASCII bytes written (excluding the NUL).
 *
 * Wide characters whose codepoint < 0x80 pass through; everything else is
 * treated as a delimiter that stops the transcription. */
static size_t wparse_copy_(const wchar_t* s, char* buf, size_t cap)
{
    size_t k = 0;

    /* Leading whitespace (ASCII subset is enough for C tokens) */
    while (s[k] == L' ' || s[k] == L'\t' || s[k] == L'\n' ||
           s[k] == L'\v' || s[k] == L'\f' || s[k] == L'\r')
        ++k;

    size_t n = 0;
    for (size_t i = 0; i < k && n + 1 < cap; ++i)
        buf[n++] = (char)s[i];

    while (s[k] && n + 1 < cap) {
        wchar_t c = s[k];

        if ((c >= L'0' && c <= L'9')
            || (c >= L'a' && c <= L'z')
            || (c >= L'A' && c <= L'Z')
            || c == L'+' || c == L'-'
            || c == L'.' || c == L'(' || c == L')'
            || c == L'_') {
            buf[n++] = (char)c;
            ++k;
        }
        else {
            break;
        }
    }

    buf[n] = 0;
    return n;
}

/* Map a byte offset (from strtoX endptr) back to a wchar offset.  Each
 * ASCII byte in `buf[0..bytes)` came from exactly one wide character in
 * `s`, so the byte offset directly equals the wchar offset. */
static wchar_t* wparse_endptr_(const wchar_t* s, const char* buf, char* end)
{
    if (!end)
        return (wchar_t*)s;
    return (wchar_t*)(s + (end - buf));
}

#endif

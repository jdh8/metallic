#ifndef _CTYPE_H
#define _CTYPE_H

#ifdef __cplusplus
extern "C" {
#endif

int iscntrl(int);
int isprint(int);
int isblank(int);
int isspace(int);
int isgraph(int);

int isupper(int);
int islower(int);
int isalpha(int);

int isdigit(int);
int isxdigit(int);

int isalnum(int);
int ispunct(int);

int toupper(int);
int tolower(int);

#ifdef __cplusplus
}
#endif

#endif /* ctype.h */

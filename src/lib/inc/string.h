#ifndef	_STRING_H_
#define	_STRING_H_
#include "type.h"
#include "const.h"

#define _U	0x01	/* upper */
#define _L	0x02	/* lower */
#define _D	0x04	/* digit */
#define _C	0x08	/* cntrl */
#define _P	0x10	/* punct */
#define _S	0x20	/* white space (space/lf/tab) */
#define _X	0x40	/* hex digit */
#define _SP	0x80	/* hard space (0x20) */

#define __ismask(x) (_ctype[(int)(unsigned char)(x)])

#define isalnum(c)	((__ismask(c)&(_U|_L|_D)) != 0)
#define isalpha(c)	((__ismask(c)&(_U|_L)) != 0)
#define iscntrl(c)	((__ismask(c)&(_C)) != 0)
#define isdigit(c)	((__ismask(c)&(_D)) != 0)
#define isgraph(c)	((__ismask(c)&(_P|_U|_L|_D)) != 0)
#define islower(c)	((__ismask(c)&(_L)) != 0)
#define isprint(c)	((__ismask(c)&(_P|_U|_L|_D|_SP)) != 0)
#define ispunct(c)	((__ismask(c)&(_P)) != 0)
#define isspace(c)	((__ismask(c)&(_S)) != 0)
#define isupper(c)	((__ismask(c)&(_U)) != 0)
#define isxdigit(c)	((__ismask(c)&(_D|_X)) != 0)

#define isascii(c) (((unsigned char)(c))<=0x7f)
#define toascii(c) (((unsigned char)(c))&0x7f)

#define tolower(c) __tolower(c)
#define toupper(c) __toupper(c)

int strnicmp(const char *s1, const char *s2, u32 len);
char * strcpy(char * dest,const char *src);
char * strncpy(char * dest,const char *src,u32 count);
char * strcat(char * dest, const char * src);
char * strncat(char *dest, const char *src, u32 count);
int strcmp(const char * cs,const char * ct);
int strncmp(const char * cs,const char * ct,u32 count);
char * strchr(const char * s, int c);
char * strrchr(const char * s, int c);
u32 strlen(const char * s);
u32 strnlen(const char * s, u32 count);
u32 strspn(const char *s, const char *accept);
char * strpbrk(const char * cs,const char * ct);
char * strtok(char * s,const char * ct);
static inline unsigned char __toupper(unsigned char c);
static inline unsigned char __tolower(unsigned char c);

#endif /* _STRING_H_ */

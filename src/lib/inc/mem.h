#ifndef	_MEM_H_
#define	_MEM_H_
#include "type.h"
#include "const.h"
#include "string.h"

void *memchr(const void *s, int c, u32 n);
char * strstr(const char * s1,const char * s2);
void * memscan(void * addr, int c, u32 size);
int memcmp(const void *cs, const void * ct, u32 count);
void * memmove(void * dest,const void *src, u32 count);
void * memcpy(void * dest,const void *src, u32 count);
char * bcopy(const char * src, char * dest, u32 count);
void * memset(void * s,char c,u32 count);


#endif /* _MEM_H_ */

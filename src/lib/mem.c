#include "inc/mem.h"
void * memset(void * s,char c,u32 count)
{
	char *xs = (char *) s;

	while (count--)
		*xs++ = c;

	return s;
}

char * bcopy(const char * src, char * dest, u32 count)
{
	char *tmp = dest;

	while (count--)
		*tmp++ = *src++;

	return dest;
}

void * memcpy(void * dest,const void *src, u32 count)
{
	char *tmp = (char *) dest, *s = (char *) src;

	while (count--)
		*tmp++ = *s++;

	return dest;
}

void * memmove(void * dest,const void *src, u32 count)
{
	char *tmp, *s;

	if (dest <= src) {
		tmp = (char *) dest;
		s = (char *) src;
		while (count--)
			*tmp++ = *s++;
		}
	else {
		tmp = (char *) dest + count;
		s = (char *) src + count;
		while (count--)
			*--tmp = *--s;
		}

	return dest;
}

int memcmp(const void *cs, const void * ct, u32 count)
{
	const unsigned char *su1, *su2;
	char res = 0;

	for( su1 = cs, su2 = ct; 0 < count; ++su1, ++su2, count--)
		if ((res = *su1 - *su2) != 0)
			break;
	return (res);
}

void * memscan(void * addr, int c, u32 size)
{
	unsigned char * p = (unsigned char *) addr;

	while (size) {
		if (*p == c)
			return (void *) p;
		p++;
		size--;
	}
  	return (void *) p;
}

char * strstr(const char * s1,const char * s2)
{
	int l1, l2;

	l2 = strlen(s2);
	if (!l2)
		return (char *) s1;
	l1 = strlen(s1);
	while (l1 >= l2) {
		l1--;
		if (!memcmp(s1,s2,l2))
			return (char *) s1;
		s1++;
	}
	return NULL;
}

void *memchr(const void *s, int c, u32 n)
{
	const unsigned char *p = s;
	while (n-- != 0) {
        	if ((unsigned char)c == *p++) {
			return (void *)p-1;
		}
	}
	return NULL;
}

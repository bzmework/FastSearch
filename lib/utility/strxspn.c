
/*
参考：
GNU C Library: http://www.gnu.org
ChromeOS: https://www.chromium.org/chromium-os
WindowsNT: https://github.com/stephanosio
SanOS: http://www.jbox.dk/sanos/
ReactOS: https://reactos.org/

来源： ChromeOS
功能： 供 strspn, strspn, strpbrk 使用
参数： 
返回： 
说明： 
示例： 
*/

#include <string.h>
#include "../typedef.h"

#ifndef LONG_BIT
#ifndef CHAR_BIT
#define CHAR_BIT 8  // number of bits in a char
#endif
#define LONG_BIT (CHAR_BIT*sizeof(long))
#endif

void* qmemset(void* dst, int32 value, tint n);

_INLINE_ void set_bit(ulong* bitmap, uint32 bit)
{
	bitmap[bit / LONG_BIT] |= 1UL << (bit % LONG_BIT);
}

_INLINE_ int32 calc_bit(ulong* bitmap, uint32 bit)
{
	return (int32)(bitmap[bit / LONG_BIT] >> (bit % LONG_BIT)) & 1;
}

uint32 qstrxspn(const tchar* str, const tchar* map, int32 parity)
{
	ulong matchmap[((1 << CHAR_BIT) + LONG_BIT - 1) / LONG_BIT];
	uint32 n = 0;

	/* Create bitmap */
	qmemset(matchmap, 0, sizeof matchmap);
	while (*map)
		set_bit(matchmap, (utchar)* map++);

	/* Make sure the null character never matches */
	if (parity)
		set_bit(matchmap, 0);

	/* Calculate span length */
	while (calc_bit(matchmap, (utchar)* str++) ^ parity)
		n++;

	return n;
}

uint32 qastrxspn(const char* str, const char* map, int32 parity)
{
	ulong matchmap[((1 << CHAR_BIT) + LONG_BIT - 1) / LONG_BIT];
	uint32 n = 0;

	/* Create bitmap */
	qmemset(matchmap, 0, sizeof matchmap);
	while (*map)
		set_bit(matchmap, (uchar)*map++);

	/* Make sure the null character never matches */
	if (parity)
		set_bit(matchmap, 0);

	/* Calculate span length */
	while (calc_bit(matchmap, (uchar)*str++) ^ parity)
		n++;

	return n;
}

uint32 qwstrxspn(const wchar* str, const wchar* map, int32 parity)
{
	ulong matchmap[((1 << CHAR_BIT) + LONG_BIT - 1) / LONG_BIT];
	uint32 n = 0;

	/* Create bitmap */
	qmemset(matchmap, 0, sizeof matchmap);
	while (*map)
		set_bit(matchmap, (wchar)*map++);

	/* Make sure the null character never matches */
	if (parity)
		set_bit(matchmap, 0);

	/* Calculate span length */
	while (calc_bit(matchmap, (wchar)*str++) ^ parity)
		n++;

	return n;
}

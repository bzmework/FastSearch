
/*
参考：
GNU C Library: http://www.gnu.org
ChromeOS: https://www.chromium.org/chromium-os
WindowsNT: https://github.com/stephanosio
SanOS: http://www.jbox.dk/sanos/
ReactOS: https://reactos.org/

来源： WindowsNT, SanOS
功能： 拷贝指定数量的源字符串到目标字符串中。
参数： dst，目标字符串；
	  src，源字符串；
	  n，字符数量。
返回： 无论拷贝是否成功，都返回目标字符串dst的地址。
说明： 必须保证目标字符串有足够的空间用来容纳源字符串。
*/

#include "../typedef.h"

tchar* qstrncpy(tchar* dst, const tchar* src, uint32 n)
{
	/*
	WindowsNT:

	tchar* start = dst;
	while (n && (*dst++ = *src++)) n--; //copy string
	if (n) while (--n) *dst++ = NUL; //pad out with zeroes
	return start;
	*/

	uint32 count = n;
	tchar* pdst = dst;
	const tchar* psrc = src;
	register tchar ch = *psrc;

	//拷贝n个字符
	while (ch && count)
	{
		*pdst++ = ch;
		ch = *(++psrc);
		count--;
	}

	//多余部分用0填充
	if (count)
	{
		while (--count)
		{
			*pdst++ = NUL;
		}
	}

	//返回目标dst
	return dst;
}

char* qastrncpy(char* dst, const char* src, uint32 n)
{
	uint32 count = n;
	char* pdst = dst;
	const char* psrc = src;
	register char ch = *psrc;

	//拷贝n个字符
	while (ch && count)
	{
		*pdst++ = ch;
		ch = *(++psrc);
		count--;
	}

	//多余部分用0填充
	if (count)
	{
		while (--count)
		{
			*pdst++ = NUL;
		}
	}

	//返回目标dst
	return dst;
}

wchar* qwstrncpy(wchar* dst, const wchar* src, uint32 n)
{
	uint32 count = n;
	wchar* pdst = dst;
	const wchar* psrc = src;
	register wchar ch = *psrc;

	//拷贝n个字符
	while (ch && count)
	{
		*pdst++ = ch;
		ch = *(++psrc);
		count--;
	}

	//多余部分用0填充
	if (count)
	{
		while (--count)
		{
			*pdst++ = NUL;
		}
	}

	//返回目标dst
	return dst;
}


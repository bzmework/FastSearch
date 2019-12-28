
/*
功能： 拷贝指定长度的源字符串到目标字符串中。
参数： dst，目标字符串；
	  src，源字符串；
	  n，目标字符串的长度（字符数量），用strlen计算得到。
返回： 返回拷贝的字符数量
说明： strlcpy是strcpy函数的安全版本，目的是防止dst目标缓冲区溢出。
*/

#include "../typedef.h"

uint32 qstrlcpy(tchar* dst, const tchar* src, uint32 n)
{
	uint32 count = 0;
	tchar* pdst = dst;
	const tchar* psrc = src;
	register tchar ch = *psrc;

	while (ch && (count < n)) //未超出目标字符串的长度才拷贝
	{
		*pdst++ = ch; 
		ch = *(++psrc); 
		count++;
	}

	*pdst = NUL;
	return count;
}

uint32 qastrlcpy(char* dst, const char* src, uint32 n)
{
	uint32 count = 0;
	char* pdst = dst;
	const char* psrc = src;
	register char ch = *psrc;

	while (ch && (count < n)) //未超出目标字符串的长度才拷贝
	{
		*pdst++ = ch;
		ch = *(++psrc);
		count++;
	}

	*pdst = NUL;
	return count;
}

uint32 qwstrlcpy(wchar* dst, const wchar* src, uint32 n)
{
	uint32 count = 0;
	wchar* pdst = dst;
	const wchar* psrc = src;
	register wchar ch = *psrc;

	while (ch && (count < n)) //未超出目标字符串的长度才拷贝
	{
		*pdst++ = ch;
		ch = *(++psrc);
		count++;
	}

	*pdst = NUL;
	return count;
}

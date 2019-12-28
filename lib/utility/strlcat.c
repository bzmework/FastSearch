
/*
功能： 拷贝指定长度的源字符串到目标字符串的末尾(即部分连接两个字符串)。
参数： dst，目标字符串；
	  src，源字符串；
	  n，指dst目标字符串的长度（字符数量），用strlen计算得到。
返回： 返回成功拷贝的字符数量。
说明： strlcat是strcat函数的安全版本，目的是防止dst目标缓冲区溢出。
*/

#include "../typedef.h"

uint32 qstrlcat(tchar* dst, const tchar* src, uint32 n)
{
	uint32 count = 0;
	tchar* pdst = dst;
	const tchar* psrc = src;
	register tchar ch;

	//定位到dst的尾部
	ch = *pdst;
	while (ch && (count < n))
	{
		ch = *(++pdst);
		count++;
	}

	//未超出目标字符串的长度才拷贝
	ch = *psrc;
	while (ch && (count < n))
	{
		*pdst++ = ch;
		ch = *(++psrc);
		count++;
	}

	*pdst = NUL;
	return count;
}

uint32 qastrlcat(char* dst, const char* src, uint32 n)
{
	uint32 count = 0;
	char* pdst = dst;
	const char* psrc = src;
	register char ch;

	//定位到dst的尾部
	ch = *pdst;
	while (ch && (count < n))
	{
		ch = *(++pdst);
		count++;
	}

	//未超出目标字符串的长度才拷贝
	ch = *psrc;
	while (ch && (count < n))
	{
		*pdst++ = ch;
		ch = *(++psrc);
		count++;
	}

	*pdst = NUL;
	return count;
}

uint32 qwstrlcat(wchar* dst, const wchar* src, uint32 n)
{
	uint32 count = 0;
	wchar* pdst = dst;
	const wchar* psrc = src;
	register wchar ch;

	//定位到dst的尾部
	ch = *pdst;
	while (ch && (count < n))
	{
		ch = *(++pdst);
		count++;
	}

	//未超出目标字符串的长度才拷贝
	ch = *psrc;
	while (ch && (count < n))
	{
		*pdst++ = ch;
		ch = *(++psrc);
		count++;
	}

	*pdst = NUL;
	return count;
}

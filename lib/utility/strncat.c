
/*
功能： 拷贝指定数量的源字符串到目标字符串的末尾(即连接两个字符串)。
参数： dst，目标字符串；
	  src，源字符串。
返回： 无论拷贝是否成功，都返回目标字符串dst的地址。
说明： 必须保证目标字符串有足够的空间用来容纳源字符串。
*/

#include "../typedef.h"

tchar* qstrncat(tchar* dst, const tchar* src, uint32 n)
{
	/*
	WindwosNT:

	tchar* start = dst;

	while (*dst++);
	dst--;

	while (n--)
	{
		if (!(*dst++ = *src++)) return start;
	}

	*dst = NUL;
	return start;
	*/
	
	tchar* pdst = dst;
	const tchar* psrc = src;
	register tchar ch = *pdst;

	//定位到dst的尾部
	while (ch)
	{
		ch = *(++pdst);
	}

	//拷贝src到dst的尾部
	while (n--)
	{
		*pdst++ = ch = *psrc++;
		if(!ch) return dst;
	}

	//返回目标dst
	*pdst = NUL;
	return dst;
}

char* qastrncat(char* dst, const char* src, uint32 n)
{
	char* pdst = dst;
	const char* psrc = src;
	register char ch = *pdst;

	//定位到dst的尾部
	while (ch)
	{
		ch = *(++pdst);
	}

	//拷贝src到dst的尾部
	while (n--)
	{
		*pdst++ = ch = *psrc++;
		if (!ch) return dst;
	}

	//返回目标dst
	*pdst = NUL;
	return dst;
}

wchar* qwstrncat(wchar* dst, const wchar* src, uint32 n)
{
	wchar* pdst = dst;
	const wchar* psrc = src;
	register wchar ch = *pdst;

	//定位到dst的尾部
	while (ch)
	{
		ch = *(++pdst);
	}

	//拷贝src到dst的尾部
	while (n--)
	{
		*pdst++ = ch = *psrc++;
		if (!ch) return dst;
	}

	//返回目标dst
	*pdst = NUL;
	return dst;
}

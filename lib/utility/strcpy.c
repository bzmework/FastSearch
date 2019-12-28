
/*
功能： 拷贝源字符串到目标字符串中。
参数： dst，目标字符串；
	  src，源字符串；
返回： 无论拷贝是否成功，都返回目标字符串dst的地址。
说明： 必须保证目标字符串有足够的空间用来容纳源字符串。
*/

#include "../typedef.h"

tchar* qstrcpy(tchar* dst, const tchar* src)
{
	/*
	WindowsNT: 

	register char* d = dst;
	while (*d++ = *src++);
	return dst;
	*/

	tchar* pdst = dst;
	const tchar* psrc = src;
	register tchar ch;

	do
	{
		*pdst++ = ch = *psrc++;
	} while (ch);

	return dst;
}

char* qastrcpy(char* dst, const char* src)
{
	char* pdst = dst;
	const char* psrc = src;
	register char ch;

	do
	{
		*pdst++ = ch = *psrc++;
	} while (ch);

	return dst;
}

wchar* qwstrcpy(wchar* dst, const wchar* src)
{
	wchar* pdst = dst;
	const wchar* psrc = src;
	register wchar ch;

	do
	{
		*pdst++ = ch = *psrc++;
	} while (ch);

	return dst;
}

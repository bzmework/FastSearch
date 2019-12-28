
/*
功能： 拷贝源字符串到目标字符串的末尾(即连接两个字符串)。
参数： dst，目标字符串；
	  src，源字符串。
返回： 无论拷贝是否成功，都返回目标字符串dst的地址。
说明： 必须保证目标字符串有足够的空间用来容纳源字符串。
*/

#include "../typedef.h"

tchar* qstrcat(tchar* dst, const tchar* src)
{
	/*
	WindowsNT,SanOS:

	char* cp = dst;
	while (*cp) cp++;		// find end of dst 
	while (*cp++ = *src++);	// Copy src to end of dst 
	return(dst);			// return dst 
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
	do
	{
		*pdst++ = ch = *psrc++; 
	} while (ch);

	//返回目标dst
	return dst;			
}

char* qastrcat(char* dst, const char* src)
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
	do
	{
		*pdst++ = ch = *psrc++;
	} while (ch);

	//返回目标dst
	return dst;
}

wchar* qwstrcat(wchar* dst, const wchar* src)
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
	do
	{
		*pdst++ = ch = *psrc++;
	} while (ch);

	//返回目标dst
	return dst;
}

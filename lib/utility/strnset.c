
/*
功能： 将字符串str中的n个字符设置成字符c。
参数： str，字符串;
	  c，要设置的字符；
	  n，要设置的字符数量。
返回： 返回字符串str的指针。
*/

#include "../typedef.h"

tchar* qstrnset(tchar* str, int32 c, uint32 n)
{
	/*
	WindowsNT:

	tchar* start = str;
	register tchar ch = (tchar)c;

	while (n-- && *str)
		*str++ = ch;

	return start;

	*/

	uint32 count = 0;
	tchar* pstr = str;
	register tchar ch = *pstr;

	while (ch && (count < n))
	{
		*pstr = (tchar)c;
		ch = *(++pstr);
		count++;
	}

	return str;
}

char* qastrnset(char* str, int32 c, uint32 n)
{
	uint32 count = 0;
	char* pstr = str;
	register char ch = *pstr;

	while (ch && (count < n))
	{
		*pstr = (char)c;
		ch = *(++pstr);
		count++;
	}

	return str;
}

wchar* qwstrnset(wchar* str, int32 c, uint32 n)
{
	uint32 count = 0;
	wchar* pstr = str;
	register wchar ch = *pstr;

	while (ch && (count < n))
	{
		*pstr = (wchar)c;
		ch = *(++pstr);
		count++;
	}

	return str;
}
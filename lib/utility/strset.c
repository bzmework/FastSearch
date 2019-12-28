
/*
参考：
GNU C Library: http://www.gnu.org
ChromeOS: https://www.chromium.org/chromium-os
WindowsNT: https://github.com/stephanosio
SanOS: http://www.jbox.dk/sanos/
ReactOS: https://reactos.org/

来源： WindowsNT
功能： 将字符串str中的所有字符都设置成字符c。
参数： str，字符串;
      c，要设置的字符。
返回： 返回字符串str的指针。
*/

#include "../typedef.h"
 
tchar* qstrset(tchar* str, int32 c)
{
	/*
	WindowsNT:

	tchar* start = str;
	register tchar ch = (tchar)c;

	while (*str)
		*str++ = ch;

	return start;

	*/

	tchar* pstr = str;
	register tchar ch = *pstr;

	while (ch)
	{
		*pstr = (tchar)c;
		ch = *(++pstr);
	}
	
	return str;
}

char* qastrset(char* str, int32 c)
{
	char* pstr = str;
	register char ch = *pstr;

	while (ch)
	{
		*pstr = (char)c;
		ch = *(++pstr);
	}

	return str;
}

wchar* qwstrset(wchar* str, int32 c)
{
	wchar* pstr = str;
	register wchar ch = *pstr;

	while (ch)
	{
		*pstr = (wchar)c;
		ch = *(++pstr);
	}

	return str;
}

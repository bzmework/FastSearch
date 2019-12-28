
/*
参考：
GNU C Library: http://www.gnu.org
ChromeOS: https://www.chromium.org/chromium-os
WindowsNT: https://github.com/stephanosio
SanOS: http://www.jbox.dk/sanos/
ReactOS: https://reactos.org/

来源： WindowsNT
功能： 字符串反转
参数： str，字符串;
返回： 返回字符串str的指针。
*/

#include "../typedef.h"

tchar* qstrrev(tchar* str)
{
	tchar* start = str;
	tchar* left = str;
	register tchar ch = *str;

	if (ch)
	{
		//定位到字符串末端
		while (ch)
		{
			ch = *(++str);
		}
		str--; //忽略NUL
		while (left < str) //首尾交换,依次循环
		{
			ch = *left;
			*left++ = *str;
			*str-- = ch;
		}
	}

	return start;
}

char* qastrrev(char* str)
{
	char* start = str;
	char* left = str;
	register char ch = *str;

	if (ch)
	{
		//定位到字符串末端
		while (ch)
		{
			ch = *(++str);
		}
		str--; //忽略NUL
		while (left < str) //首尾交换,依次循环
		{
			ch = *left;
			*left++ = *str;
			*str-- = ch;
		}
	}

	return start;
}

wchar* qwstrrev(wchar* str)
{
	wchar* start = str;
	wchar* left = str;
	register wchar ch = *str;

	if (ch)
	{
		//定位到字符串末端
		while (ch)
		{
			ch = *(++str);
		}
		str--; //忽略NUL
		while (left < str) //首尾交换,依次循环
		{
			ch = *left;
			*left++ = *str;
			*str-- = ch;
		}
	}

	return start;
}

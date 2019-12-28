
/*
�ο���
GNU C Library: http://www.gnu.org
ChromeOS: https://www.chromium.org/chromium-os
WindowsNT: https://github.com/stephanosio
SanOS: http://www.jbox.dk/sanos/
ReactOS: https://reactos.org/

��Դ�� WindowsNT
���ܣ� ���ַ���str�е������ַ������ó��ַ�c��
������ str���ַ���;
      c��Ҫ���õ��ַ���
���أ� �����ַ���str��ָ�롣
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

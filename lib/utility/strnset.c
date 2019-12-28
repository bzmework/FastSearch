
/*
���ܣ� ���ַ���str�е�n���ַ����ó��ַ�c��
������ str���ַ���;
	  c��Ҫ���õ��ַ���
	  n��Ҫ���õ��ַ�������
���أ� �����ַ���str��ָ�롣
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
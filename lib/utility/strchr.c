
/*
���ܣ� �����ַ���str���״γ����ַ�c��λ�á�
������ str���ַ���;
      c�����ҵ��ַ���
���أ� ����ָ���ַ����е�һ�γ����ַ�c��ָ�룬
	  ����ַ�����û�г����ַ�c���򷵻�NULL��
*/

#include "../typedef.h"

tchar* qstrchr(const tchar* str, int32 c)
{
	/*
	WindowsNT,SanOS:

	register tchar ch;
	while (*str && *str != ch)
		str++;

	if (*str == ch)
		return ((tchar*)str);

	return NULL;
	*/

	const tchar* pstr = str;
	register tchar ch = *pstr;

	while (ch)
	{
		if (ch == (tchar)c)
			return ((tchar*)pstr);
		ch = *(++pstr);
	}

	return NULL;
}

char* qastrchr(const char* str, int32 c)
{
	const char* pstr = str;
	register char ch = *pstr;

	while (ch)
	{
		if (ch == (char)c)
			return ((char*)pstr);
		ch = *(++pstr);
	}

	return NULL;
}

wchar* qwstrchr(const wchar* str, int32 c)
{
	const wchar* pstr = str;
	register wchar ch = *pstr;

	while (ch)
	{
		if (ch == (wchar)c)
			return ((wchar*)pstr);
		ch = *(++pstr);
	}

	return NULL;
}




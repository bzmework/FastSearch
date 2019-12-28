
/*
���ܣ� �����ַ���str�����һ�γ����ַ�c��λ�á�
������ str���ַ���;
	  c�����ҵ��ַ���
���أ� ����ָ���ַ��������γ����ַ�c��ָ�룬
	  ����ַ�����û�г����ַ�c���򷵻�NULL��
*/

#include "../typedef.h"

tchar* qstrrchr(const tchar* str, int32 c)
{
	/*
	Windows NT��
	ȱ�ݣ����Ƚ�ָ�붨λ��ĩβ���������ǰ���ң����û���ҵ��أ���ɶ�����ȫ�����ķ��ա�
	tchar* start = (tchar*)str;
	tchar ch = (tchar)c;

	while (*str++) //��λ���ַ���ĩβ
		;

	while (--str != start && *str != ch) //��ǰ����
		;

	if (*str == ch) //�ҵ��ַ�
		return ((tchar*)str);

	return NULL;
	*/

	const tchar* found = NULL;
	const tchar* pstr = str;
	tchar ch = *pstr;

	while (ch)
	{
		if (ch == (tchar)c)
			found = pstr;
		ch = *(++pstr);
	}

	return (tchar*)found;
}

char* qastrrchr(const char* str, int32 c)
{
	const char* found = NULL;
	const char* pstr = str;
	char ch = *pstr;

	while (ch)
	{
		if (ch == (char)c)
			found = pstr;
		ch = *(++pstr);
	}

	return (char*)found;
}

wchar* qwstrrchr(const wchar* str, int32 c)
{
	const wchar* found = NULL;
	const wchar* pstr = str;
	wchar ch = *pstr;

	while (ch)
	{
		if (ch == (wchar)c)
			found = pstr;
		ch = *(++pstr);
	}

	return (wchar*)found;
}


/*
���ܣ� �ַ����Ƚϣ����ִ�Сд���������ƱȽϡ�
������ str1���ַ���1��
	  str2���ַ���2��
���أ� ��� str1 > str2 ����ֵ > 0
	  ��� str1 < str2 ����ֵ < 0
	  ��� str1 = str2 ����ֵ = 0
*/

#include "../typedef.h"

int32 qstrcmp(const tchar* str1, const tchar* str2)
{
	/*
	WindowsNT, SanOS:

	int32 ret = 0;
	while (!(ret = *(utchar*) str1 - *(utchar*) str2) && *str2)
	  ++str1, ++str2;

	if (ret < 0)
		ret = -1;
	else if (ret > 0)
		ret = 1 ;

	return ret;
	*/

	//ΪʲôҪת����unsigned char����ΪҪ����Ƚϴ�С��
	const utchar* s1 = (const utchar*)str1; 
	const utchar* s2 = (const utchar*)str2;
	register utchar c1, c2;

	do
	{
		c1 = (utchar)*s1++;
		c2 = (utchar)*s2++;
		if (!c1) 
			return c1 - c2;
	} while (c1 == c2);

	return c1 - c2;
}

int32 qastrcmp(const char* str1, const char* str2)
{
	//ΪʲôҪת����unsigned char����ΪҪ����Ƚϴ�С��
	const uchar* s1 = (const uchar*)str1;
	const uchar* s2 = (const uchar*)str2;
	register uchar c1, c2;

	do
	{
		c1 = (uchar)* s1++;
		c2 = (uchar)* s2++;
		if (!c1)
			return c1 - c2;
	} while (c1 == c2);

	return c1 - c2;
}

int32 qwstrcmp(const wchar* str1, const wchar* str2)
{
	const wchar* s1 = str1;
	const wchar* s2 = str2;
	register wchar c1, c2;

	do
	{
		c1 = (wchar)*s1++;
		c2 = (wchar)*s2++;
		if (!c1)
			return c1 - c2;
	} while (c1 == c2);

	return c1 - c2;
}

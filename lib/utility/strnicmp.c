
/*
���ܣ� �Ƚ�ָ�����ȵ��ַ����������ִ�Сд�����ı��Ƚϡ�
������ str1���ַ���1��
	  str2���ַ���2��
	  n���Ƚϵ��ַ�����
���أ� ��� str1 > str2 ����ֵ > 0
	  ��� str1 < str2 ����ֵ < 0
	  ��� str1 = str2 ����ֵ = 0
˵����
*/

#include "../typedef.h"

int32 qstrnicmp(const tchar* str1, const tchar* str2, uint32 n)
{
	const utchar* c1 = (const utchar*)str1;
	const utchar* c2 = (const utchar*)str2;
	int32 f = 0, l = 0;

	do
	{
		if (((f = *(c1++)) >= __T('A')) && (f <= __T('Z')))
			f -= __T('A') - __T('a');

		if (((l = *(c2++)) >= __T('A')) && (l <= __T('Z')))
			l -= __T('A') - __T('a');
	} while (--n && f && (f == l));

	return (f - l);
}

int32 qastrnicmp(const char* str1, const char* str2, uint32 n)
{
	const uchar* c1 = (const uchar*)str1;
	const uchar* c2 = (const uchar*)str2;
	int32 f = 0, l = 0;

	do
	{
		if (((f = *(c1++)) >= 'A') && (f <= 'Z'))
			f -= 'A' - 'a';

		if (((l = *(c2++)) >= 'A') && (l <= 'Z'))
			l -= 'A' - 'a';
	} while (--n && f && (f == l));

	return (f - l);
}

int32 qwstrnicmp(const wchar* str1, const wchar* str2, uint32 n)
{
	const wchar* c1 = str1;
	const wchar* c2 = str2;
	int32 f = 0, l = 0;

	do
	{
		if (((f = *(c1++)) >= __W('A')) && (f <= __W('Z')))
			f -= __W('A') - __W('a');

		if (((l = *(c2++)) >= __W('A')) && (l <= __W('Z')))
			l -= __W('A') - __W('a');
	} while (--n && f && (f == l));

	return (f - l);
}

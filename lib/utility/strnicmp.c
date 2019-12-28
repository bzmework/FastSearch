
/*
功能： 比较指定长度的字符串，不区分大小写，即文本比较。
参数： str1，字符串1；
	  str2，字符串2；
	  n，比较的字符数量
返回： 如果 str1 > str2 返回值 > 0
	  如果 str1 < str2 返回值 < 0
	  如果 str1 = str2 返回值 = 0
说明：
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

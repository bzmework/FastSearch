
/*
功能： 字符串比较，不区分大小写，即文本比较。
参数： str1，字符串1；
	  str2，字符串2。
返回： 如果 str1 > str2 返回值 > 0
	  如果 str1 < str2 返回值 < 0
	  如果 str1 = str2 返回值 = 0
说明：  
*/

#include "../typedef.h"

int32 qstricmp(const tchar* str1, const tchar* str2)
{
	//为什么要转换成unsigned char，因为要相减比较大小。
	const utchar* c1 = (const utchar*)str1;
	const utchar* c2 = (const utchar*)str2;
	int32 f = 0, l = 0;

	do
	{
		if (((f = *(c1++)) >= __T('A')) && (f <= __T('Z')))
			f -= (__T('A') - __T('a'));

		if (((l = *(c2++)) >= __T('A')) && (l <= __T('Z')))
			l -= (__T('A') - __T('a'));
	} while (f && (f == l));

	return (f - l);
}

int32 qastricmp(const char* str1, const char* str2)
{
	//为什么要转换成unsigned char，因为要相减比较大小。
	const uchar* c1 = (const uchar*)str1;
	const uchar* c2 = (const uchar*)str2;
	int32 f = 0, l = 0;

	do
	{
		if (((f = *(c1++)) >= 'A') && (f <= 'Z'))
			f -= ('A' - 'a');

		if (((l = *(c2++)) >= 'A') && (l <= 'Z'))
			l -= ('A' - 'a');
	} while (f && (f == l));

	return (f - l);
}

int32 qwstricmp(const wchar* str1, const wchar* str2)
{
	const wchar* c1 = str1;
	const wchar* c2 = str2;
	int32 f = 0, l = 0;

	do
	{
		if (((f = *(c1++)) >= __W('A')) && (f <= __W('Z')))
			f -= (__W('A') - __W('a'));

		if (((l = *(c2++)) >= __W('A')) && (l <= __W('Z')))
			l -= (__W('A') - __W('a'));
	} while (f && (f == l));

	return (f - l);
}
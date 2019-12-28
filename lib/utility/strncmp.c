
/*
参考：
GNU C Library: http://www.gnu.org
ChromeOS: https://www.chromium.org/chromium-os
WindowsNT: https://github.com/stephanosio
SanOS: http://www.jbox.dk/sanos/
ReactOS: https://reactos.org/

来源： ChromeOS
功能： 比较指定长度的字符串，区分大小写，即二进制比较。
参数： str1，字符串1；
	  str2，字符串2；
	  n，比较的字符数量
返回： 如果 str1 > str2 返回值 > 0
	  如果 str1 < str2 返回值 < 0
	  如果 str1 = str2 返回值 = 0
*/

#include "../typedef.h"

int32 qstrncmp(const tchar* str1, const tchar* str2, uint32 n)
{
	/*
	WindwosNT：

	if (!n) return 0;
	while (--n && *str1 && *str1 == *str2)
	{
		str1++;
		str2++;
	}
	return *(utchar*) str1 - *(utchar*) str2;
	*/

	//为什么要转换成unsigned char，因为要相减比较大小。
	if (!n) return 0;
	const utchar* c1 = (const utchar*)str1;
	const utchar* c2 = (const utchar*)str2;
	register utchar ch;
	int32 d = 0;

	while (n--)
	{
		d = (int32)(ch = *c1++) - (int32)*c2++;
		if (d || !ch)
			break;
	}

	return d;
}

int32 qastrncmp(const char* str1, const char* str2, uint32 n)
{
	//为什么要转换成unsigned char，因为要相减比较大小。
	if (!n) return 0;
	const uchar* c1 = (const uchar*)str1;
	const uchar* c2 = (const uchar*)str2;
	register uchar ch;
	int32 d = 0;

	while (n--)
	{
		d = (int32)(ch = *c1++) - (int32)* c2++;
		if (d || !ch)
			break;
	}

	return d;
}

int32 qwstrncmp(const wchar* str1, const wchar* str2, uint32 n)
{
	if (!n) return 0;
	const wchar* c1 = str1;
	const wchar* c2 = str2;
	register wchar ch;
	int32 d = 0;

	while (n--)
	{
		d = (int32)(ch = *c1++) - (int32)*c2++;
		if (d || !ch)
			break;
	}

	return d;
}

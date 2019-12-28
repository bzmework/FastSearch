
/*
功能： 查找字符串str中最后一次出现字符c的位置。
参数： str，字符串;
	  c，查找的字符。
返回： 返回指向字符串中最后次出现字符c的指针，
	  如果字符串中没有出现字符c，则返回NULL。
*/

#include "../typedef.h"

tchar* qstrrchr(const tchar* str, int32 c)
{
	/*
	Windows NT：
	缺陷：首先将指针定位到末尾，其次再向前查找，如果没有找到呢？造成二次完全搜索的风险。
	tchar* start = (tchar*)str;
	tchar ch = (tchar)c;

	while (*str++) //定位到字符串末尾
		;

	while (--str != start && *str != ch) //向前搜索
		;

	if (*str == ch) //找到字符
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

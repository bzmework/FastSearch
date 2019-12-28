
/*
来源： WindowsNT
功能： 查找字符串str2是否存在于字符串str1中。
参数： str1，字符串1;
	  str2，字符串2。
返回： 返回指向第一次出现字符串str2的指针，
	  如果str2不存在于str1中，则返回NULL。
*/

#include "../typedef.h"

tchar* qstrstr(const tchar* str1, const tchar* str2)
{
	/*
	register const unsigned int len = strlen(str2);

	if (len == 0) return (char*)str1;
	while (*str1 != *str2 || strncmp(str1, str2, len))
		if (*str1++ == '\0')
			return (char*)NULL;
	return (char*)str1;
	*/

	tchar* cp = (tchar*)str1;
	tchar* s1, *s2;

	if (!(*str2))
		return ((tchar*)str1);

	while (*cp)
	{
		s1 = cp;
		s2 = (tchar*)str2;

		while (*s1 && *s2 && !(*s1 - *s2))
			s1++, s2++;

		if (!(*s2))
			return cp;

		cp++;
	}

	return NULL;
}

char* qastrstr(const char* str1, const char* str2)
{
	char* cp = (char*)str1;
	char* s1, * s2;

	if (!(*str2))
		return ((char*)str1);

	while (*cp)
	{
		s1 = cp;
		s2 = (char*)str2;

		while (*s1 && *s2 && !(*s1 - *s2))
			s1++, s2++;

		if (!(*s2))
			return cp;

		cp++;
	}

	return NULL;
}

wchar* qwstrstr(const wchar* str1, const wchar* str2)
{
	wchar* cp = (wchar*)str1;
	wchar* s1, * s2;

	if (!(*str2))
		return ((wchar*)str1);

	while (*cp)
	{
		s1 = cp;
		s2 = (wchar*)str2;

		while (*s1 && *s2 && !(*s1 - *s2))
			s1++, s2++;

		if (!(*s2))
			return cp;

		cp++;
	}

	return NULL;
}


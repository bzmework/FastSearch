
/*
来源： ChromeOS
功能： 查询字符串str开头连续包含字符串accept的子字符串的字符数量。即字符串对齐连续匹配。
参数： str，字符串;
	  accept，要匹配的字符串。
返回： 返回字符串str开头连续包含字符串accept内的字符数量。
示例： char* str =       "The Linux was first developed for 386/486-based pcs.";
	  len = strspn(str, "The Linux was"); 返回14，说明有10个字符连续匹配。
	  len = strspn(str, "The Linux"); 返回10，说明有10个字符连续匹配。
	  len = strspn(str, "The"); 返回3，说明有10个字符连续匹配。
	  len = strspn(str, "Linux was"); 返回0，未连续匹配，虽然看起来连续，但未对齐。
	  len = strspn(str, "was"); 返回0，未连续匹配，与上相同，未对齐。
*/

#include "../typedef.h"
//#include "strxspn.c"

uint32 qstrxspn(const tchar* str, const tchar* map, int32 parity);
uint32 qastrxspn(const char* str, const char* map, int32 parity);
uint32 qwstrxspn(const wchar* str, const wchar* map, int32 parity);

uint32 qstrspn(const tchar* str, const tchar* accept)
{
	/*
	register const char* s1, * s2;

	for (s1 = str1; *s1; s1++) 
	{
		for (s2 = str2; *s2 && *s2 != *s1; s2++)
			;
		if (*s2 == '\0')
			break;
	}
	return s1 - str1;
	*/

	return qstrxspn(str, accept, 0);
}

uint32 qastrspn(const char* str, const char* accept)
{
	return qastrxspn(str, accept, 0);
}

uint32 qwstrspn(const wchar* str, const wchar* accept)
{
	return qwstrxspn(str, accept, 0);
}

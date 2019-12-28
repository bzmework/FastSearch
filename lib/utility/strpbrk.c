
/*
来源： ChromeOS
功能： 查找字符串str中出现accept中任意字符时的位置。
参数： str，字符串;
	  accept，要匹配的字符串。
返回： 如果找到，返回找到字符所在位置的指针;否则返回NULL。
示例： char* p = NULL;
	  char* str = "The Linux was first developed for 386/486-based pcs.";
	  p = strpbrk(str1, "ix"); 返回"inux was first developed for 386/486-based pcs."
	  p = strpbrk(str1, "lop"); 返回"loped for 386/486-based pcs."
	  p = strpbrk(str1, "jk"); 返回NULL
*/

#include "../typedef.h"
//#include "strxspn.c"

uint32 qstrxspn(const tchar* str, const tchar* map, int32 parity);
uint32 qastrxspn(const char* str, const char* map, int32 parity);
uint32 qwstrxspn(const wchar* str, const wchar* map, int32 parity);

tchar* qstrpbrk(const tchar* str, const tchar* accept)
{
	const tchar* ps = str + qstrxspn(str, accept, 1);
	return *ps ? (tchar*)ps : NULL;
}

char* qastrpbrk(const char* str, const char* accept)
{
	const char* ps = str + qastrxspn(str, accept, 1);
	return *ps ? (char*)ps : NULL;
}

wchar* qwstrpbrk(const wchar* str, const wchar* accept)
{
	const wchar* ps = str + qwstrxspn(str, accept, 1);
	return *ps ? (wchar*)ps : NULL;
}

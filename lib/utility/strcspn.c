
/*
来源： ChromeOS
功能： 查询字符串str中首次出现字符串reject中的任意字符的位置。
参数： str，字符串;
	  reject，要匹配的字符串。
返回： 返回字符串str开头连续不包含字符串reject内的字符数量。
说明： strcspn是由string complementary span的缩写组成，表示获取字符串str起始位置起不在字符串reject中的字符的数量。
示例： char* str = "The Linux was first developed for 386/486-based pcs.";
	  len = strcspn(str1, " "); 返回3，返回首次出现" "(空格)时的长度，所以返回3。
	  len = strcspn(str1, "/-"); 返回37，返回首次出现"/"或"-"时的长度，所以返回37。
	  len = strcspn(str1, "1234567890"); 返回34，返回首次出现0-9之间的任意数值时的长度，所以返回34。
	  len = strcspn(str1, "jk"); 返回52，没有找到首次出现的字符，则返回整个字符串的长度，所以返回52。
*/

#include "../typedef.h"

uint32 qstrxspn(const tchar* str, const tchar* map, int32 parity);
uint32 qastrxspn(const char* str, const char* map, int32 parity);
uint32 qwstrxspn(const wchar* str, const wchar* map, int32 parity);

uint32 qstrcspn(const tchar* str, const tchar* reject)
{
	return qstrxspn(str, reject, 1);
}

uint32 qastrcspn(const char* str, const char* reject)
{
	return qastrxspn(str, reject, 1);
}

uint32 qwstrcspn(const wchar* str, const wchar* reject)
{
	return qwstrxspn(str, reject, 1);
}

/*
来源： ChromeOS
功能： 字符串分割。
参数： pstr，字符串地址;
	  delim，分割字符串。
返回： 返回str起始位置的指针。
说明： 从字符串str指向的位置起向后扫描，遇到delim字符串中的任意字符后，将此字符替换为NUL，并结束分割，
      然后将str指针定位在分割处，以便对str继续进行分割，
      最后返回str起始位置的指针，以便输出分割好的字符串。
示例： char str[] = "hello&world&你好&世界"; 
	  char* q = str;
	  char* p;
	  while (p = strsep(&q, "&"))
      {
         printf("%s\n", p);
      }
      输出：
      hello
      world
      你好
      世界
注意： 传入的字符串必须是可修改的，例如数组或malloc分配的动态内存，不能传入常量字符串。
*/

#include "../typedef.h"

tchar* qstrpbrk(const tchar* str, const tchar* accept);
char* qastrpbrk(const char* str, const char* accept);
wchar* qwstrpbrk(const wchar* str, const wchar* accept);

tchar* qstrsep(tchar** pstr, const tchar* delim)
{
	tchar* s = *pstr;
	tchar* e;

	if (!s) //字符串不能为空
		return NULL;

	e = qstrpbrk(s, delim); //查找delim
	if (e)
		*e++ = NUL;

	*pstr = e;//将指针定位到分割位置，以便于对str继续进行分割
	return s;//返回str起始位置的指针，以便输出分割好的字符串
}

char* qastrsep(char** pstr, const char* delim)
{
	char* s = *pstr;
	char* e;

	if (!s) //字符串不能为空
		return NULL;

	e = qastrpbrk(s, delim); //查找delim
	if (e)
		* e++ = NUL;

	*pstr = e;//将指针定位到分割位置，以便于对str继续进行分割
	return s;//返回str起始位置的指针，以便输出分割好的字符串
}

wchar* qwstrsep(wchar** pstr, const wchar* delim)
{
	wchar* s = *pstr;
	wchar* e;

	if (!s) //字符串不能为空
		return NULL;

	e = qwstrpbrk(s, delim); //查找delim
	if (e)
		* e++ = NUL;

	*pstr = e;//将指针定位到分割位置，以便于对str继续进行分割
	return s;//返回str起始位置的指针，以便输出分割好的字符串
}

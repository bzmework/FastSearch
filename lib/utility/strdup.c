
/*
功能： 返回字符串的拷贝。
参数： str，字符串
返回： 成功返回指向字符串拷贝的指针，失败返回NULL。
说明： strdup()在内部调用了malloc()为变量分配内存，不需要使用返回的字符串时，必须用free()释放相应的内存空间，否则会造成内存泄漏。
*/

#include <malloc.h>
#include "../typedef.h"

uint32 qstrlen(const tchar* str);
uint32 qastrlen(const char* str);
uint32 qwstrlen(const wchar* str);

void* qmemcpy(void* dst, const void* src, tint n);

tchar* qstrdup(const tchar* str)
{
	if (!str) return NULL;
	uint32 len = (qstrlen(str) + 1) * sizeof(tchar); //+1是为了拷贝结束符NUL, 乘以sizeof(tchar)是为了将数量转换成字节
	tchar* t = (tchar*)malloc(len);
	if (t) qmemcpy(t, str, len);
	return t;
}

char* qastrdup(const char* str)
{
	if (!str) return NULL;
	uint32 len = (qastrlen(str) + 1) * sizeof(char);
	char* t = (char*)malloc(len);
	if (t) qmemcpy(t, str, len);
	return t;
}

wchar* qwstrdup(const wchar* str)
{
	if (!str) return NULL;
	uint32 len = (qwstrlen(str) + 1) * sizeof(wchar);
	wchar* t = (wchar*)malloc(len);
	if (t) qmemcpy(t, str, len);
	return t;
}

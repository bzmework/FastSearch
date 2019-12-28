
/*
功能： 返回指定数量的字符串拷贝。
参数： str，字符串
返回： 成功返回指向字符串拷贝的指针，失败返回NULL。
说明： strndup()在内部调用了malloc()为变量分配内存，不需要使用返回的字符串时，必须用free()释放相应的内存空间，否则会造成内存泄漏。
*/

#include <malloc.h>
#include "../typedef.h"
//#include "strlen.c"

uint32 qstrlen(const tchar* str);
uint32 qastrlen(const char* str);
uint32 qwstrlen(const wchar* str);

void* qmemcpy(void* dst, const void* src, tint n);

tchar* qstrndup(const tchar* str, uint32 n)
{
	uint32 l = qstrlen(str);
	uint32 len = (n > l ? l + 1: n + 1) * sizeof(tchar); //+1是为了拷贝结束符NUL, 乘以sizeof(tchar)是为了将数量转换成字节
	tchar* d = (tchar*)malloc(len);
	if (d)
	{
		qmemcpy(d, str, len);
		d[n] = NUL;
	}
	return d;
}

char* qastrndup(const char* str, uint32 n)
{
	uint32 l = qastrlen(str);
	uint32 len = (n > l ? l + 1 : n + 1) * sizeof(char);
	char* d = (char*)malloc(len);
	if (d)
	{
		qmemcpy(d, str, len);
		d[n] = NUL;
	}
	return d;
}

wchar* qwstrndup(const wchar* str, uint32 n)
{
	uint32 l = qwstrlen(str);
	uint32 len = (n > l ? l + 1 : n + 1) * sizeof(wchar);
	wchar* d = (wchar*)malloc(len);
	if (d)
	{
		qmemcpy(d, str, len);
		d[n] = NUL;
	}
	return d;
}


/*
���ܣ� ����ָ���������ַ���������
������ str���ַ���
���أ� �ɹ�����ָ���ַ���������ָ�룬ʧ�ܷ���NULL��
˵���� strndup()���ڲ�������malloc()Ϊ���������ڴ棬����Ҫʹ�÷��ص��ַ���ʱ��������free()�ͷ���Ӧ���ڴ�ռ䣬���������ڴ�й©��
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
	uint32 len = (n > l ? l + 1: n + 1) * sizeof(tchar); //+1��Ϊ�˿���������NUL, ����sizeof(tchar)��Ϊ�˽�����ת�����ֽ�
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

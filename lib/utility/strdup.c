
/*
���ܣ� �����ַ����Ŀ�����
������ str���ַ���
���أ� �ɹ�����ָ���ַ���������ָ�룬ʧ�ܷ���NULL��
˵���� strdup()���ڲ�������malloc()Ϊ���������ڴ棬����Ҫʹ�÷��ص��ַ���ʱ��������free()�ͷ���Ӧ���ڴ�ռ䣬���������ڴ�й©��
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
	uint32 len = (qstrlen(str) + 1) * sizeof(tchar); //+1��Ϊ�˿���������NUL, ����sizeof(tchar)��Ϊ�˽�����ת�����ֽ�
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

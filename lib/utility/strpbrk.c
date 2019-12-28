
/*
��Դ�� ChromeOS
���ܣ� �����ַ���str�г���accept�������ַ�ʱ��λ�á�
������ str���ַ���;
	  accept��Ҫƥ����ַ�����
���أ� ����ҵ��������ҵ��ַ�����λ�õ�ָ��;���򷵻�NULL��
ʾ���� char* p = NULL;
	  char* str = "The Linux was first developed for 386/486-based pcs.";
	  p = strpbrk(str1, "ix"); ����"inux was first developed for 386/486-based pcs."
	  p = strpbrk(str1, "lop"); ����"loped for 386/486-based pcs."
	  p = strpbrk(str1, "jk"); ����NULL
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

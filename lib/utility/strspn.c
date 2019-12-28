
/*
��Դ�� ChromeOS
���ܣ� ��ѯ�ַ���str��ͷ���������ַ���accept�����ַ������ַ����������ַ�����������ƥ�䡣
������ str���ַ���;
	  accept��Ҫƥ����ַ�����
���أ� �����ַ���str��ͷ���������ַ���accept�ڵ��ַ�������
ʾ���� char* str =       "The Linux was first developed for 386/486-based pcs.";
	  len = strspn(str, "The Linux was"); ����14��˵����10���ַ�����ƥ�䡣
	  len = strspn(str, "The Linux"); ����10��˵����10���ַ�����ƥ�䡣
	  len = strspn(str, "The"); ����3��˵����10���ַ�����ƥ�䡣
	  len = strspn(str, "Linux was"); ����0��δ����ƥ�䣬��Ȼ��������������δ���롣
	  len = strspn(str, "was"); ����0��δ����ƥ�䣬������ͬ��δ���롣
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

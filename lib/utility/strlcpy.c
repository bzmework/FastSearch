
/*
���ܣ� ����ָ�����ȵ�Դ�ַ�����Ŀ���ַ����С�
������ dst��Ŀ���ַ�����
	  src��Դ�ַ�����
	  n��Ŀ���ַ����ĳ��ȣ��ַ�����������strlen����õ���
���أ� ���ؿ������ַ�����
˵���� strlcpy��strcpy�����İ�ȫ�汾��Ŀ���Ƿ�ֹdstĿ�껺���������
*/

#include "../typedef.h"

uint32 qstrlcpy(tchar* dst, const tchar* src, uint32 n)
{
	uint32 count = 0;
	tchar* pdst = dst;
	const tchar* psrc = src;
	register tchar ch = *psrc;

	while (ch && (count < n)) //δ����Ŀ���ַ����ĳ��Ȳſ���
	{
		*pdst++ = ch; 
		ch = *(++psrc); 
		count++;
	}

	*pdst = NUL;
	return count;
}

uint32 qastrlcpy(char* dst, const char* src, uint32 n)
{
	uint32 count = 0;
	char* pdst = dst;
	const char* psrc = src;
	register char ch = *psrc;

	while (ch && (count < n)) //δ����Ŀ���ַ����ĳ��Ȳſ���
	{
		*pdst++ = ch;
		ch = *(++psrc);
		count++;
	}

	*pdst = NUL;
	return count;
}

uint32 qwstrlcpy(wchar* dst, const wchar* src, uint32 n)
{
	uint32 count = 0;
	wchar* pdst = dst;
	const wchar* psrc = src;
	register wchar ch = *psrc;

	while (ch && (count < n)) //δ����Ŀ���ַ����ĳ��Ȳſ���
	{
		*pdst++ = ch;
		ch = *(++psrc);
		count++;
	}

	*pdst = NUL;
	return count;
}

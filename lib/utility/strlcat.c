
/*
���ܣ� ����ָ�����ȵ�Դ�ַ�����Ŀ���ַ�����ĩβ(���������������ַ���)��
������ dst��Ŀ���ַ�����
	  src��Դ�ַ�����
	  n��ָdstĿ���ַ����ĳ��ȣ��ַ�����������strlen����õ���
���أ� ���سɹ��������ַ�������
˵���� strlcat��strcat�����İ�ȫ�汾��Ŀ���Ƿ�ֹdstĿ�껺���������
*/

#include "../typedef.h"

uint32 qstrlcat(tchar* dst, const tchar* src, uint32 n)
{
	uint32 count = 0;
	tchar* pdst = dst;
	const tchar* psrc = src;
	register tchar ch;

	//��λ��dst��β��
	ch = *pdst;
	while (ch && (count < n))
	{
		ch = *(++pdst);
		count++;
	}

	//δ����Ŀ���ַ����ĳ��Ȳſ���
	ch = *psrc;
	while (ch && (count < n))
	{
		*pdst++ = ch;
		ch = *(++psrc);
		count++;
	}

	*pdst = NUL;
	return count;
}

uint32 qastrlcat(char* dst, const char* src, uint32 n)
{
	uint32 count = 0;
	char* pdst = dst;
	const char* psrc = src;
	register char ch;

	//��λ��dst��β��
	ch = *pdst;
	while (ch && (count < n))
	{
		ch = *(++pdst);
		count++;
	}

	//δ����Ŀ���ַ����ĳ��Ȳſ���
	ch = *psrc;
	while (ch && (count < n))
	{
		*pdst++ = ch;
		ch = *(++psrc);
		count++;
	}

	*pdst = NUL;
	return count;
}

uint32 qwstrlcat(wchar* dst, const wchar* src, uint32 n)
{
	uint32 count = 0;
	wchar* pdst = dst;
	const wchar* psrc = src;
	register wchar ch;

	//��λ��dst��β��
	ch = *pdst;
	while (ch && (count < n))
	{
		ch = *(++pdst);
		count++;
	}

	//δ����Ŀ���ַ����ĳ��Ȳſ���
	ch = *psrc;
	while (ch && (count < n))
	{
		*pdst++ = ch;
		ch = *(++psrc);
		count++;
	}

	*pdst = NUL;
	return count;
}

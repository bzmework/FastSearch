
/*
���ܣ� ����Դ�ַ�����Ŀ���ַ����С�
������ dst��Ŀ���ַ�����
	  src��Դ�ַ�����
���أ� ���ۿ����Ƿ�ɹ���������Ŀ���ַ���dst�ĵ�ַ��
˵���� ���뱣֤Ŀ���ַ������㹻�Ŀռ���������Դ�ַ�����
*/

#include "../typedef.h"

tchar* qstrcpy(tchar* dst, const tchar* src)
{
	/*
	WindowsNT: 

	register char* d = dst;
	while (*d++ = *src++);
	return dst;
	*/

	tchar* pdst = dst;
	const tchar* psrc = src;
	register tchar ch;

	do
	{
		*pdst++ = ch = *psrc++;
	} while (ch);

	return dst;
}

char* qastrcpy(char* dst, const char* src)
{
	char* pdst = dst;
	const char* psrc = src;
	register char ch;

	do
	{
		*pdst++ = ch = *psrc++;
	} while (ch);

	return dst;
}

wchar* qwstrcpy(wchar* dst, const wchar* src)
{
	wchar* pdst = dst;
	const wchar* psrc = src;
	register wchar ch;

	do
	{
		*pdst++ = ch = *psrc++;
	} while (ch);

	return dst;
}

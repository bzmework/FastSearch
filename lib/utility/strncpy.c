
/*
�ο���
GNU C Library: http://www.gnu.org
ChromeOS: https://www.chromium.org/chromium-os
WindowsNT: https://github.com/stephanosio
SanOS: http://www.jbox.dk/sanos/
ReactOS: https://reactos.org/

��Դ�� WindowsNT, SanOS
���ܣ� ����ָ��������Դ�ַ�����Ŀ���ַ����С�
������ dst��Ŀ���ַ�����
	  src��Դ�ַ�����
	  n���ַ�������
���أ� ���ۿ����Ƿ�ɹ���������Ŀ���ַ���dst�ĵ�ַ��
˵���� ���뱣֤Ŀ���ַ������㹻�Ŀռ���������Դ�ַ�����
*/

#include "../typedef.h"

tchar* qstrncpy(tchar* dst, const tchar* src, uint32 n)
{
	/*
	WindowsNT:

	tchar* start = dst;
	while (n && (*dst++ = *src++)) n--; //copy string
	if (n) while (--n) *dst++ = NUL; //pad out with zeroes
	return start;
	*/

	uint32 count = n;
	tchar* pdst = dst;
	const tchar* psrc = src;
	register tchar ch = *psrc;

	//����n���ַ�
	while (ch && count)
	{
		*pdst++ = ch;
		ch = *(++psrc);
		count--;
	}

	//���ಿ����0���
	if (count)
	{
		while (--count)
		{
			*pdst++ = NUL;
		}
	}

	//����Ŀ��dst
	return dst;
}

char* qastrncpy(char* dst, const char* src, uint32 n)
{
	uint32 count = n;
	char* pdst = dst;
	const char* psrc = src;
	register char ch = *psrc;

	//����n���ַ�
	while (ch && count)
	{
		*pdst++ = ch;
		ch = *(++psrc);
		count--;
	}

	//���ಿ����0���
	if (count)
	{
		while (--count)
		{
			*pdst++ = NUL;
		}
	}

	//����Ŀ��dst
	return dst;
}

wchar* qwstrncpy(wchar* dst, const wchar* src, uint32 n)
{
	uint32 count = n;
	wchar* pdst = dst;
	const wchar* psrc = src;
	register wchar ch = *psrc;

	//����n���ַ�
	while (ch && count)
	{
		*pdst++ = ch;
		ch = *(++psrc);
		count--;
	}

	//���ಿ����0���
	if (count)
	{
		while (--count)
		{
			*pdst++ = NUL;
		}
	}

	//����Ŀ��dst
	return dst;
}


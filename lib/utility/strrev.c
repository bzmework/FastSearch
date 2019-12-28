
/*
�ο���
GNU C Library: http://www.gnu.org
ChromeOS: https://www.chromium.org/chromium-os
WindowsNT: https://github.com/stephanosio
SanOS: http://www.jbox.dk/sanos/
ReactOS: https://reactos.org/

��Դ�� WindowsNT
���ܣ� �ַ�����ת
������ str���ַ���;
���أ� �����ַ���str��ָ�롣
*/

#include "../typedef.h"

tchar* qstrrev(tchar* str)
{
	tchar* start = str;
	tchar* left = str;
	register tchar ch = *str;

	if (ch)
	{
		//��λ���ַ���ĩ��
		while (ch)
		{
			ch = *(++str);
		}
		str--; //����NUL
		while (left < str) //��β����,����ѭ��
		{
			ch = *left;
			*left++ = *str;
			*str-- = ch;
		}
	}

	return start;
}

char* qastrrev(char* str)
{
	char* start = str;
	char* left = str;
	register char ch = *str;

	if (ch)
	{
		//��λ���ַ���ĩ��
		while (ch)
		{
			ch = *(++str);
		}
		str--; //����NUL
		while (left < str) //��β����,����ѭ��
		{
			ch = *left;
			*left++ = *str;
			*str-- = ch;
		}
	}

	return start;
}

wchar* qwstrrev(wchar* str)
{
	wchar* start = str;
	wchar* left = str;
	register wchar ch = *str;

	if (ch)
	{
		//��λ���ַ���ĩ��
		while (ch)
		{
			ch = *(++str);
		}
		str--; //����NUL
		while (left < str) //��β����,����ѭ��
		{
			ch = *left;
			*left++ = *str;
			*str-- = ch;
		}
	}

	return start;
}

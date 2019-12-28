
/*
���ܣ� ����Դ�ַ�����Ŀ���ַ�����ĩβ(�����������ַ���)��
������ dst��Ŀ���ַ�����
	  src��Դ�ַ�����
���أ� ���ۿ����Ƿ�ɹ���������Ŀ���ַ���dst�ĵ�ַ��
˵���� ���뱣֤Ŀ���ַ������㹻�Ŀռ���������Դ�ַ�����
*/

#include "../typedef.h"

tchar* qstrcat(tchar* dst, const tchar* src)
{
	/*
	WindowsNT,SanOS:

	char* cp = dst;
	while (*cp) cp++;		// find end of dst 
	while (*cp++ = *src++);	// Copy src to end of dst 
	return(dst);			// return dst 
	*/

	tchar* pdst = dst;
	const tchar* psrc = src;
	register tchar ch = *pdst;

	//��λ��dst��β��
	while (ch)
	{
		ch = *(++pdst);
	}

	//����src��dst��β��
	do
	{
		*pdst++ = ch = *psrc++; 
	} while (ch);

	//����Ŀ��dst
	return dst;			
}

char* qastrcat(char* dst, const char* src)
{
	char* pdst = dst;
	const char* psrc = src;
	register char ch = *pdst;

	//��λ��dst��β��
	while (ch)
	{
		ch = *(++pdst);
	}

	//����src��dst��β��
	do
	{
		*pdst++ = ch = *psrc++;
	} while (ch);

	//����Ŀ��dst
	return dst;
}

wchar* qwstrcat(wchar* dst, const wchar* src)
{
	wchar* pdst = dst;
	const wchar* psrc = src;
	register wchar ch = *pdst;

	//��λ��dst��β��
	while (ch)
	{
		ch = *(++pdst);
	}

	//����src��dst��β��
	do
	{
		*pdst++ = ch = *psrc++;
	} while (ch);

	//����Ŀ��dst
	return dst;
}

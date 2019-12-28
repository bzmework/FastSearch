
/*
���ܣ� ����ָ��������Դ�ַ�����Ŀ���ַ�����ĩβ(�����������ַ���)��
������ dst��Ŀ���ַ�����
	  src��Դ�ַ�����
���أ� ���ۿ����Ƿ�ɹ���������Ŀ���ַ���dst�ĵ�ַ��
˵���� ���뱣֤Ŀ���ַ������㹻�Ŀռ���������Դ�ַ�����
*/

#include "../typedef.h"

tchar* qstrncat(tchar* dst, const tchar* src, uint32 n)
{
	/*
	WindwosNT:

	tchar* start = dst;

	while (*dst++);
	dst--;

	while (n--)
	{
		if (!(*dst++ = *src++)) return start;
	}

	*dst = NUL;
	return start;
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
	while (n--)
	{
		*pdst++ = ch = *psrc++;
		if(!ch) return dst;
	}

	//����Ŀ��dst
	*pdst = NUL;
	return dst;
}

char* qastrncat(char* dst, const char* src, uint32 n)
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
	while (n--)
	{
		*pdst++ = ch = *psrc++;
		if (!ch) return dst;
	}

	//����Ŀ��dst
	*pdst = NUL;
	return dst;
}

wchar* qwstrncat(wchar* dst, const wchar* src, uint32 n)
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
	while (n--)
	{
		*pdst++ = ch = *psrc++;
		if (!ch) return dst;
	}

	//����Ŀ��dst
	*pdst = NUL;
	return dst;
}


/*
���ܣ� ��dstĿ���ڴ�������n���ֽڵ�c�滻��������ָ��dstĿ���ڴ������ָ�롣
������ dst��Ŀ���ڴ�����
	  value��Ҫ�滻��ֵ;
	  n, �滻���ֽ�����
���أ� ����ָ��Ŀ���ڴ������ָ�롣
˵����
*/

#include "../typedef.h"

void* qmemset(void* dst, int32 value, tint n)
{
	char* pdst = dst;
	int32 count = n;

	for (; count; count--)
	{
		*pdst++ = value;
	}
	return dst;
}

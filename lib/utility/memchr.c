
/*
���ܣ� ��buf�ڴ������е�n���ֽ��в����ַ�ch��
������ buf���ڴ�����;
	  c�����ҵ��ַ���
���أ� ����ָ�򻺴��е�һ�γ����ַ�c��ָ�룬
	  ���������û�г����ַ�c���򷵻�NULL��
*/

#include "../typedef.h"

void* qmemchr(const void* buf, int chr, tint n)
{
	uchar* pbuff;
	tint count = n;

	for (pbuff = (uchar*)buf; count; count--)
	{
		if (*pbuff == (uchar)chr)
			break;
		pbuff++;
	}
	return count ? (void*)pbuff : NULL;
}

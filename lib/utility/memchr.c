
/*
功能： 从buf内存区域中的n个字节中查找字符ch。
参数： buf，内存区域;
	  c，查找的字符。
返回： 返回指向缓存中第一次出现字符c的指针，
	  如果缓存中没有出现字符c，则返回NULL。
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

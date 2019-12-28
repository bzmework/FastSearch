
/*
功能： 将dst目标内存区域用n个字节的c替换，并返回指向dst目标内存区域的指针。
参数： dst，目标内存区域；
	  value，要替换的值;
	  n, 替换的字节数。
返回： 返回指向目标内存区域的指针。
说明：
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

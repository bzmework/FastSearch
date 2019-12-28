
/*
功能： 将src所指内存区域复制n个字节到dst所指内存区域中，并返回指向dst目标区域的指针。
参数： dst，目标内存区域；
	  src，源内存区域;
	  n, 拷贝的字节数。
返回： 返回指向目标内存区域的指针。
说明： memmove能够保证源区域在被覆盖之前将重叠区域的字节拷贝到目标区域中，但复制后源内容会被更改。
      但是当目标区域与源区域没有重叠时，则和memcpy函数的功能相同。
*/

#include "../typedef.h"

void* qmemmove(void* dst, const void* src, tint n)
{
	void* ret = dst;

	if (n < 4) //按字节拷贝
	{
		char* pdst = (char*)dst;
		char* psrc = (char*)src;

		int32 count = n;
		for (; count; count--)
		{
			*pdst++ = *psrc++; //copy from lower addresses to higher addresses
		}
	}
	else //n >= 4 对齐快速拷贝
	{
		if (dst <= src || (char*)dst >= ((char*)src + n))
		{
			//非重叠缓冲区
			//Non-Overlapping Buffers
			//copy from lower addresses to higher addresses

			//拷贝0-len之间的数据
			int32* pndst = (int32*)dst; //转换为4字节整型指针
			int32* pnsrc = (int32*)src;
			int32 len = n >> 2; //整除，相当于：(int)(n / 4);
			for (; len; len--)
			{
				*pndst++ = *pnsrc++; //copy from lower addresses to higher addresses
			}

			//将剩余的数据按字节拷贝
			int32 rem = n & 3; //求余，相当于：n % 4；
			if (rem)
			{
				char* pcdst = (char*)pndst; 
				char* pcsrc = (char*)pnsrc;
				for (; rem; rem--)
				{
					*pcdst++ = *pcsrc++; //copy from lower addresses to higher addresses
				}
			}
		}
		else
		{
			//重叠缓冲区,即同一缓冲区
			//Overlapping Buffers
			//copy from higher addresses to lower addresses

			//拷贝0-len之间的数据
			int32* pndst = (int32*)((char*)dst + n); //转换为4字节整型指针
			int32* pnsrc = (int32*)((char*)src + n);
			int32 len = n >> 2; //整除，相当于：(int)(n / 4);
			for (; len; len--)
			{
				*--pndst = *--pnsrc; //copy from lower addresses to higher addresses
			}

			//将剩余的数据按字节拷贝
			int32 rem = n & 3; //求余，相当于：n % 4；
			if (rem)
			{
				char* pcdst = (char*)pndst;
				char* pcsrc = (char*)pnsrc;
				for (; rem; rem--)
				{
					*--pcdst = *--pcsrc; //copy from lower addresses to higher addresses
				}
			}
		}
	}

	//返回
	return ret;
}
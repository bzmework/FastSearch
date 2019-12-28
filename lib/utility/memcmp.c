
/*
功能： 将src源内存区域与dst目标内存区域的n个字节进行比较
参数： dst，目标内存区域；
	  src，源内存区域;
	  n, 比较的字节数。
返回： 如果 dst > src 返回值 > 0
	  如果 dst < src 返回值 < 0
	  如果 dst = src 返回值 = 0
说明：
*/

#include <emmintrin.h>
#include "../typedef.h"

int32 qmemcmp(const void* dst, const void* src, tint n)
{
	//基本判断
	//if (!n) //为零
	//	return 0;
	//if (dst == src) //相同的内存区域
	//	return 0;

	//参考: https://macosxfilerecovery.com/faster-memory-comparison-in-c/
	//     正如作者所说简单优化后要比一些标准库的传统算法快35%，
	//     这是因为现在的CPU比较两个4字节整型数比比较两个单字节字符要快，而且需要更少的CPU周期。
	//参考：http://www.picklingtools.com/study.pdf
	//     在这篇论文中作者给出了同样的结论，即4字节对齐优化算法，并给出更为具体的算法。
	//参考：GNU C Library: http://www.gnu.org
	//     glibc标准库同样是4字节对齐优化算法，但其memcmp.c给出的是一个更为完善的优化算法。
	//根据以上参考算法，编写的优化算法如下：
	if (n < 4)//按字节比较
	{
		//参考FreeBSD等各种算法后，优化的传统算法如下：
		const uchar* pdst = (uchar*)dst;
		const uchar* psrc = (uchar*)src;
		tint count = n;
		for (; count; count--)
		{
			if (*pdst != *psrc)
				return (uchar)*pdst - (uchar)*psrc;
			pdst++;
			psrc++;
		}
	}
	else //n >= 4 对齐快速比较
	{
		//比较0-len之间的数据
		const int32* pndst = (int32*)dst; //转换为4字节整型指针
		const int32* pnsrc = (int32*)src;
		int32 len = n >> 2; //整除，相当于：(int)(n / 4);
		for (; len; len--)
		{
			if (*pndst != *pnsrc) 
				return (uchar)*pndst - (uchar)*pnsrc;
			pndst++;
			pnsrc++;
		}

		//将剩余的最后4个字节按照传统算法比较
		const uint8* pcdst = (uint8*)pndst; 
		const uint8* pcsrc = (uint8*)pnsrc;
		int32 rem = n & 3; //求余，相当于：n % 4；
		for (; rem; rem--)
		{
			if (*pcdst != *pcsrc) 
				return (uchar)*pcdst - (uchar)*pcsrc;
			pcdst++;
			pcsrc++;
		}
	}

	//相等
	return 0;
}
 

/*
功能： 将src所指源内存区域复制n个字节到dst所指的目标内存区域中，并返回指向dst目标内存区域的指针。
参数： dst，目标内存区域；
	  src，源内存区域;
	  n, 拷贝的字节数。
返回： 返回指向目标内存区域的指针。
说明： 编译时注意在C/C++命令行加上：/arch:SSE2 参见：https://docs.microsoft.com/en-us/cpp/build/reference/arch-x86?view=vs-2019
*/

#include <emmintrin.h>
#include <string.h>
#include "../typedef.h"

//拷贝小数据(<= 128 字节), 按4字节对齐拷贝
_INLINE_ void* qmemcpys(void* dst, const void* src, tint n)
{
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
			len = n >> 2;
			char* pcdst = (char*)pndst;
			char* pcsrc = (char*)pnsrc;
			for (; rem; rem--)
			{
				*pcdst++ = *pcsrc++; //copy from lower addresses to higher addresses
			}
		}
	}

	//返回
	return dst;
}

//参考：https://github.com/skywind3000/FastMemcpy
//拷贝大数据(>128字节), 按16字节对齐拷贝
_INLINE_ void* qmemcpyl(void* dst, const void* src, tint n)
{
	static tint cachesize = 0x200000; // L2-cache size

	void* ret = dst;
	char* pdst = (char*)dst;
	const char* psrc = (const char*)src;
	tint padding = 0;
	tint count = n;

	//必须将目标对齐到16字节边界
	padding = (16 - (((tint)pdst) & 15)) & 15;
	if (padding > 0)
	{
		__m128i head = _mm_loadu_si128((const __m128i*)psrc);
		_mm_storeu_si128((__m128i*)pdst, head);
		pdst += padding;
		psrc += padding;
		count -= padding;
	}

	// 中等大小的数据拷贝
	if (count <= cachesize)
	{
		__m128i c0, c1, c2, c3, c4, c5, c6, c7;

		for (; count >= 128; count -= 128)
		{
			//读128字节
			c0 = _mm_loadu_si128(((const __m128i*)psrc) + 0);
			c1 = _mm_loadu_si128(((const __m128i*)psrc) + 1);
			c2 = _mm_loadu_si128(((const __m128i*)psrc) + 2);
			c3 = _mm_loadu_si128(((const __m128i*)psrc) + 3);
			c4 = _mm_loadu_si128(((const __m128i*)psrc) + 4);
			c5 = _mm_loadu_si128(((const __m128i*)psrc) + 5);
			c6 = _mm_loadu_si128(((const __m128i*)psrc) + 6);
			c7 = _mm_loadu_si128(((const __m128i*)psrc) + 7);

			//预取
			_mm_prefetch((const char*)(psrc + 256), _MM_HINT_NTA);

			//写128字节
			_mm_store_si128((((__m128i*)pdst) + 0), c0);
			_mm_store_si128((((__m128i*)pdst) + 1), c1);
			_mm_store_si128((((__m128i*)pdst) + 2), c2);
			_mm_store_si128((((__m128i*)pdst) + 3), c3);
			_mm_store_si128((((__m128i*)pdst) + 4), c4);
			_mm_store_si128((((__m128i*)pdst) + 5), c5);
			_mm_store_si128((((__m128i*)pdst) + 6), c6);
			_mm_store_si128((((__m128i*)pdst) + 7), c7);

			//下一块
			psrc += 128;
			pdst += 128;
		}

		//拷贝剩余数据
		memcpy(pdst, psrc, count);
	}
	else //大数据拷贝
	{
		__m128i c0, c1, c2, c3, c4, c5, c6, c7;

		//先预取
		_mm_prefetch((const char*)(psrc), _MM_HINT_NTA);

		//拷贝
		if ((((size_t)psrc) & 15) == 0) //源16字节对齐
		{	
			for (; count >= 128; count -= 128)
			{
				//读128字节
				c0 = _mm_load_si128(((const __m128i*)psrc) + 0);
				c1 = _mm_load_si128(((const __m128i*)psrc) + 1);
				c2 = _mm_load_si128(((const __m128i*)psrc) + 2);
				c3 = _mm_load_si128(((const __m128i*)psrc) + 3);
				c4 = _mm_load_si128(((const __m128i*)psrc) + 4);
				c5 = _mm_load_si128(((const __m128i*)psrc) + 5);
				c6 = _mm_load_si128(((const __m128i*)psrc) + 6);
				c7 = _mm_load_si128(((const __m128i*)psrc) + 7);

				//预取
				_mm_prefetch((const char*)(psrc + 256), _MM_HINT_NTA);

				//写128字节
				_mm_stream_si128((((__m128i*)pdst) + 0), c0);
				_mm_stream_si128((((__m128i*)pdst) + 1), c1);
				_mm_stream_si128((((__m128i*)pdst) + 2), c2);
				_mm_stream_si128((((__m128i*)pdst) + 3), c3);
				_mm_stream_si128((((__m128i*)pdst) + 4), c4);
				_mm_stream_si128((((__m128i*)pdst) + 5), c5);
				_mm_stream_si128((((__m128i*)pdst) + 6), c6);
				_mm_stream_si128((((__m128i*)pdst) + 7), c7);

				//下一块
				psrc += 128;
				pdst += 128;
			}
		}
		else //源未16字节对齐
		{	
			for (; count >= 128; count -= 128)
			{
				//读128字节
				c0 = _mm_loadu_si128(((const __m128i*)psrc) + 0);
				c1 = _mm_loadu_si128(((const __m128i*)psrc) + 1);
				c2 = _mm_loadu_si128(((const __m128i*)psrc) + 2);
				c3 = _mm_loadu_si128(((const __m128i*)psrc) + 3);
				c4 = _mm_loadu_si128(((const __m128i*)psrc) + 4);
				c5 = _mm_loadu_si128(((const __m128i*)psrc) + 5);
				c6 = _mm_loadu_si128(((const __m128i*)psrc) + 6);
				c7 = _mm_loadu_si128(((const __m128i*)psrc) + 7);

				//预取
				_mm_prefetch((const char*)(psrc + 256), _MM_HINT_NTA);
				
				//写入
				_mm_stream_si128((((__m128i*)pdst) + 0), c0);
				_mm_stream_si128((((__m128i*)pdst) + 1), c1);
				_mm_stream_si128((((__m128i*)pdst) + 2), c2);
				_mm_stream_si128((((__m128i*)pdst) + 3), c3);
				_mm_stream_si128((((__m128i*)pdst) + 4), c4);
				_mm_stream_si128((((__m128i*)pdst) + 5), c5);
				_mm_stream_si128((((__m128i*)pdst) + 6), c6);
				_mm_stream_si128((((__m128i*)pdst) + 7), c7);
				
				//下一块
				psrc += 128;
				pdst += 128;
			}
		}
		_mm_sfence();

		//拷贝剩余数据
		memcpy(pdst, psrc, count);
	}

	return ret;

}

//快速拷贝
//注意如果在循环内进行大数据量拷贝，应该直接调用qmemcpys/qmemcpyl，
//因为函数堆栈切换将耗用非常多的时间。
void* qmemcpy(void* dst, const void* src, tint n)
{
	if (n > 128) //大数据
	{
		return qmemcpyl(dst, src, n);
	}
	else //小数据
	{
		//测试发现系统内置的memcpy函数，在小于128字节的情况下，编译器已经做了最大优化
		//return memcpy(dst, src, n);
		return qmemcpys(dst, src, n);
	}
}
